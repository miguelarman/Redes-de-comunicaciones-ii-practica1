/**
 * @brief Este es el fichero que contiene la funcion que inicia el servidor.
 *
 * En este fichero esta la funcion principal del servidor, que lee el fichero
 * de configuracion, demoniza, crea threads que luego procesaran a los clientes.
 *
 * @file server.c
 * @author Miguel Arconada Manteca y Mario García Pascual
 * @date 9-5-2019
 */

#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>

#include "../includes/blockingQueue.h"
#include "../includes/connection.h"
#include "../includes/procesa_conexion.h"
#include "../includes/daemon.h"
#include "../includes/server.h"
#include "../includes/configParser.h"

#define SERVER_PORT 9999 /*!< Puerto del servidor */
#define SERVER_IP "127.0.0.1" /*!< Direccion IP del servidor */


int close_server = 0; /**< Bandera para cerrar el servidor */
int listenfd; /**< File descriptor devuelto por listen */
char abspath[MAX_STR]; /**< Ruta absoluta del servidor */

/**
 * @brief Funcion encargada de manejar la interrupcion.
 *
 * Esta funcion se encarga de manejar la interrucion para apagar el servidor.
 *
 * @ingroup Server
 * @param signum El numero de la interrupcion.
 * @return nada
 */
void sig_int(int signum) {
  shutdown(listenfd, SHUT_RD);      /* cierra el socket del servidor */
  close_server = 1;
  exit(1);
}

/**
 * @brief Esta estructura se pasa por la cola para que los hilos la procesen.
 *
 * @ingroup Server
 *
 * Esta es la estructura que se pasa por la cola bloqueante
 * para que los hilos la procesen
 */
typedef struct {
    int clifd;                /**< client file descriptor */
    int poisoned;             /**< indica si el paquete esta envenenado */
} client;

/**
 * @brief Funcion constructora de la estructura client.
 *
 * Construye e inicializa los parametros de la estructura client.
 *
 * @ingroup Server
 * @param c_out El cliente creado se guardara en este puntero.
 * @param clifd El descriptor de fichero para procesar la conexion del cliente.
 * @param poisoned Indica si el paquete esta envenenado o no.
 * @return ERROR si ha habido algun problema; SUCCESS en caso contrario.
 */
int client_create(client **c_out, int clifd, int poisoned)
{
  client *c;

  if (c_out == NULL) {
    return ERROR;
  }

  c = (client *)malloc(sizeof(client));
  /* falta control */
  c->clifd = clifd;
  c->poisoned = poisoned;
  *c_out = c;
  return SUCCESS;
}

/**
 * @brief Funcion destructora de la estructura client.
 *
 * Destruye y libera un objeto client.
 *
 * @ingroup Server
 * @param c El cliente a destruir.
 * @return ERROR si ha habido algun problema; SUCCESS en caso contrario.
 */
int client_destroy(client *c)
{
  if (c == NULL) {
    return ERROR;
  }

  free(c);
  return SUCCESS;
}

/*
  Rutina de los hilos
*/

/**
 * @brief Función de los hilos de la threadpool.
 *
 * Función que ejecutan los hilos de la threadpool del servidor.
 *
 * @ingroup Server
 * @param arg La cola bloqueante con la que se comuncia con el padre.
 * @return nada
 */
void *thread_main(void *arg) {
  blockingQueue *queue;
  client *c;

  /* Coge la cola del argumento */
  queue = (blockingQueue *)arg;
  while (1) {
    blockingQueue_get(queue, (void *)&c);
    if (c->poisoned == 1) {
      free(c);
      pthread_exit(NULL);
    }
    procesa_conexion(c->clifd, abspath);
    close(c->clifd);
    free(c);
  }
}

/**
 * @brief Función main.
 *
 * Función principal del servidor.
 *
 * @ingroup Server
 * @param argc Número de argumentos.
 * @param argv Argumentos.
 * @return ERROR si ha habido algun problema; SUCCESS en caso contrario.
 */
int main(int argc, char **argv)
{
  int i, connfd;
  int r;
  socklen_t addrlen, clilen;
  struct sockaddr *cliaddr;
  blockingQueue *queue;
  pthread_t threads[THREAD_COUNT];
  client **c;
  configOptions opts;

  /* Maneja la interrupcion para cerrar el servidor */
  if (signal(SIGINT, sig_int) == SIG_ERR) {
    return ERROR;
  }

  /* Se parsean los parametros del fichero de configuracion */
  if (parseConfig("server.conf", &opts) == ERROR) {
    return ERROR;
  }

  /* Se concatena el directorio de trabajo al server root */
  if (getcwd(abspath, MAX_STR) == NULL) {
    return ERROR;
  }
  strcat(abspath, opts.server_root);

  /* Demonizamos el proceso, por lo que se quedan ejecutando */
  r = demonizar();
  if (r == ES_PADRE) {
    exit(EXIT_SUCCESS);
  }


  c = (client **)malloc(sizeof(client *));

  /* Contiene las llamadas a socket(), bind() y listen() */
  listenfd = tcp_listen(SERVER_IP, SERVER_PORT, 20);

  /* Crea la cola bloqueante */
  blockingQueue_create(&queue, QUEUE_SIZE);
  /* Crea los hilos */
  for (i = 0; i < THREAD_COUNT; i++) {
    pthread_create(&threads[i], NULL, thread_main, queue);
    /* falta control */
  }

  while (1) {
    clilen = addrlen;
    connfd = accept(listenfd, cliaddr, &clilen);

    if (close_server == 1) {
      break;
    }

    client_create(c, connfd, 0);
    blockingQueue_put(queue, *c);
  }

  for (i = 0; i < THREAD_COUNT; i++) {
    client_create(c, 0, 1);
    blockingQueue_put(queue, *c);
  }

  for (i = 0; i < THREAD_COUNT; i++) {
    pthread_join(threads[i], NULL);
  }

  blockingQueue_destroy(queue);
  free(c);
  return SUCCESS;
}

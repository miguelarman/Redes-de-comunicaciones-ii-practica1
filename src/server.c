/**
 * @brief Descripción del fichero/librería
 *
 * Descripción más elaborada
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

#define SERVER_PORT 9999 /*!< Descripción de macro */
#define SERVER_IP "127.0.0.1" /*!< Descripción de macro */


int close_server = 0; /**< Descipción de la variable */
int listenfd; /**< Descipción de la variable */
char abspath[MAX_STR]; /**< Descipción de la variable */

/**
 * @brief Descripción simple de la función
 *
 * Descripción de la funcion
 *
 * @ingroup Server
 * @param signum Descripción del argumento
 * @return Descripción del retorno
 */
void sig_int(int signum) {
  shutdown(listenfd, SHUT_RD);      /* cierra el socket del servidor */
  close_server = 1;
  exit(1);
}

/**
 * @brief Descripción de la estructura
 *
 * Esta es la estructura que se pasa por la cola bloqueante
 * para que los hilos la procesen
 */
typedef struct {
    int clifd;                /**< client file descriptor */
    int poisoned;             /**< indica si el paquete esta envenado */
} client;

/**
 * @brief Descripción simple de la función
 *
 * Descripción de la funcion
 *
 * @ingroup Server
 * @param c_out Descripción del argumento
 * @param clifd Descripción del argumento
 * @param poisoned Descripción del argumento
 * @return Descripción del retorno
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
 * @brief Descripción simple de la función
 *
 * Descripción de la funcion
 *
 * @ingroup Server
 * @param c Descripción del argumento
 * @return Descripción del retorno
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
 * @brief Descripción simple de la función
 *
 * Descripción de la funcion
 *
 * @ingroup Server
 * @param arg Descripción del argumento
 * @return Descripción del retorno
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
 * @brief Descripción simple de la función
 *
 * Descripción de la funcion
 *
 * @ingroup Server
 * @param argc Descripción del argumento
 * @param argv Descripción del argumento
 * @return Descripción del retorno
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

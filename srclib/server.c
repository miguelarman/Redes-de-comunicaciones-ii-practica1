#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>

#include "../includes/blockingQueue.h"
#include "../includes/servidor_threadpool.h"

int close_server = 0;
int listendfd;

void sig_int(int signum) {
  shutdown(listenfd, SHUT_RDWR);      /* cierra el socket del servidor */
  close_server = 1;
  exit(1);
}

/*
  Esta es la estructura que se pasa por la cola bloqueante para que los hilos
  la procesen
*/

typedef struct
{
    int clifd;                /* client file descriptor */
    int poisoned;             /* indica si el paquete esta envenando */
} client;

int client_create(client **c_out, int clifd, int poisoned)
{
  client *c;

  if (c_out == NULL) {
    return ERROR;
  }

  c = (client *)malloc(sizeof(client));
  // falta control
  c->clifd = clifd;
  c->poisoned = poisoned;
  *c_out = c;
  return SUCCESS;
}

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
    process_request(connfd);
    close(clifd);
    free(c);
  }
}

int main(int argc, char **argv)
{
  int i, connfd;
  socklen_t addrlen, clilen;
  struct sockaddr *cliaddr;
  blockingQueue *queue;
  pthread_t threads[THREAD_COUNT];
  client **c;

  c = (client **)malloc(sizeof(client *));

  /* Contiene las llamadas a socket(), bind() y listen() */
  //listenfd = Tcp_listen(argv[1], argv[2], &addrlen);
  /* Crea la cola bloqueante */
  blockingQueue_create(&queue, QUEUE_SIZE);
  /* Crea los hilos */
  for (i = 0; i < THREAD_COUNT; i++) {
    pthread_create(&threads[i], NULL, thread_main, queue);
    // falta control
  }

  while (1) {
    clilen = addrlen;
    connfd = accept(listenfd, cliaddr, &clilen);

    if (close_server == 1) {
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

    client_create(c, connfd, 0);
    blockingQueue_put(queue, *c);
  }
}

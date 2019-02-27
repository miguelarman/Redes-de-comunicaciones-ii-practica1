#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/socket.h>

#include "../includes/blockingQueue.h"
#include "../includes/servidor_threadpool.h"

struct _client {
    int connfd;
    int poisoned;
};
/*
  Esta es la estructura que se pasa por la cola bloqueante para
  que los hilos la procesen
*/
int client_create(client **c_out, int connfd, int poisoned) {
  client *c;

  if (c_out == NULL) {
    return ERROR;
  }

  c = (client *)malloc(sizeof(client));
  // falta control
  c->connfd = connfd;
  c->poisoned = poisoned;
  *c_out = c;
  return SUCCESS;
}

void *thread_routine(void *arg) {
  blockingQueue *queue;
  client *c;

  /* Coge la cola del argumento */
  queue = (blockingQueue *)arg;
  while (1) {
    blockingQueue_get(queue, (void *)&c);
    if (c->poisoned == 1) {
      printf("Soy %ld y me han envenenado ahhh\n", pthread_self());
      free(c);
      pthread_exit(NULL);
    }
    //process_request(connfd);
    printf("Soy %ld y estoy procesando el cliente %d jejej\n", pthread_self(), c->connfd);
    //Close(connfd);
    free(c);
  }
}

int client_destroy(client *c) {
  if (c == NULL) {
    return ERROR;
  }
  free(c);
  return SUCCESS;
}

int main(int argc, char **argv)
{
  int listenfd, i;
  socklen_t addrlen;
  blockingQueue *queue;
  pthread_t threads[THREAD_COUNT];
  client **c;

  c = (client **)malloc(sizeof(client *));

  /* Contiene las llamadas a socket(), bind() y listen() */
  //listenfd = Tcp_listen(argv[1], argv[2], &addrlen);
  /* Crea la cola bloqueante */
  blockingQueue_create(&queue, QUEUE_SIZE);
  /* Crea los hilos y los despega */
  for (i = 0; i < THREAD_COUNT; i++) {
    pthread_create(&threads[i], NULL, thread_routine, queue);
    // falta control
    //pthread_detach(threads[i]);
    // falta control
    printf("Soy el padre y he creado y no espegado a mi hijo de nombre %ld\n", threads[i]);
  }

  printf("Soy el padre y voy a crear %d tareas para mis hijos y luego los voy a matar\n", THREAD_COUNT*2);
  for (i = 0; i < THREAD_COUNT*2; i++) {
    client_create(c, i, 0);
    blockingQueue_put(queue, *c);
  }
  for (i = 0; i < THREAD_COUNT; i++) {
    client_create(c, -1, 1);
    blockingQueue_put(queue, *c);
  }

  for (i = 0; i < THREAD_COUNT; i++) {
    pthread_join(threads[i], NULL);
  }

  blockingQueue_destroy(queue);
  free(c);
  return 0;
}

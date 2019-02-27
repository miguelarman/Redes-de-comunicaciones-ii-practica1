
#include ".../includes/blockingQueue.h"

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

int client_destroy(client *c) {
  if (client == NULL) {
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

  /* Contiene las llamadas a socket(), bind() y listen() */
  //listenfd = Tcp_listen(argv[1], argv[2], &addrlen);
  /* Crea la cola bloqueante */
  blockingQueue_create(queue, QUEUE_SIZE, THREAD_COUNT);
  /* Crea los hilos y los despega */
  for (i = 0; i < thread_count; i++) {
    pthread_create(threads[i], NULL, thread_routine, queue);
    // falta control
    pthread_detach(threads[i]);
    // falta control
    printf("Soy el padre y he creado y despegado a mi hijo de nombre %d\n", threads[i]);
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
}

void *thread_routine(void *arg) {
  blockingQueue *queue;
  client *c;

  /* Coge la cola del argumento */
  queue = (blockingQueue *)arg;
  while (1) {
    blockingQueue_get(queue, &c);
    if (c->is_poisoned == 1) {
      printf("Soy %d y me han envenenado ahhh\n", pthread_self());
      pthread_exit(NULL);
    }
    //process_request(connfd);
    printf("Soy %d y estoy procesando el cliente %d jejej\n", pthread_self(), client->connfd);
    Close(connfd);
    free(c);
  }
}

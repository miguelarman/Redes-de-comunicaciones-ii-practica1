
#include "blockingQueue.h"

struct _client {
    int connfd;
    int poisoned;
};

int client_create(client **c_out, int connfd, int poisoned) {
  client *c;

  if (c == NULL) {
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

}

int die;

int main(int argc, char **argv)
{
  int listenfd, i;
  socklen_t addrlen;
  blockingQueue *queue;
  pthread_t threads[THREAD_COUNT];
  client **c;

  /* Contiene las llamadas a socket(), bind() y listen() */
  listenfd = Tcp_listen(argv[1], argv[2], &addrlen);
  /* Crea la cola bloqueante */
  blockingQueue_create(queue, QUEUE_SIZE, THREAD_COUNT);
  /* Crea los hilos y los despega */
  for (i = 0; i < thread_count; i++) {
    pthread_create(tp->threads[i], NULL, thread_routine, queue);
    // falta control
    pthread_detach(tp->threads[i]);
    // falta control
  }

  while (1) {
    if (die == 1)
      finaliza
    connfd = Accept(listenfd, cliaddr, &clilen);
    client_create(c, connfd, 0);
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
      pthread_exit(NULL);
    }
    process_request(connfd);
    Close(connfd);
    free(c);
  }
}

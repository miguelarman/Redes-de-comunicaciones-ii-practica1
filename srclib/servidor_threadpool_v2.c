
/*
  Esta es otra version del servidor, en la que no se usa la cola bloqueante
  implementada y son los hilos los que se encargan de hacer el accept
*/

int main(int argc, char **argv)
{
  int listenfd, i;
  socklen_t addrlen;
  blockingQueue *queue;
  pthread_t threads[THREAD_COUNT];
  client **c;

  /* Contiene las llamadas a socket(), bind() y listen() */
  listenfd = Tcp_listen(argv[1], argv[2], &addrlen);
  /* Crea los hilos y los despega */
  for (i = 0; i < thread_count; i++) {
    // hay que especificar los args como aqui el hilo hace el accept necesita
    // mas info
    pthread_create(tp->threads[i], NULL, thread_routine, args);
    // falta control
    pthread_detach(tp->threads[i]);
    // falta control
  }

  while (1) {
    /* El padre se desentiende */
  }
}

void *thread_routine(void *args) {
  /* Los hilos hacen todo el trabajo */
  while (1) {
    Accept(listenfd, cliadrr, &clilen);
    process_request(connfd);
    Close(connfd);
  }
}

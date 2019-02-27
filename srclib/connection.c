#include "../includes/connection.h"

#define BACKLOG 10
#define ERROR -1


int tcp_listen(char *ip, int port, struct sockaddr* addr) {
  // TODO ¿el argumento de la ip?

  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    // TODO
  }

  if (bind(sockfd, addr, sizeof(addr)) < 0) {
    // TODO
  }

  if (listen(sockfd, BACKLOG) < 0) {
    // TODO
  }

  return ERROR;
}

int accept_connection(int sockfd, struct sockaddr* addr, socklen_t *addrlen) {
  // TODO

  return accept(sockfd, addr, addrlen);
}

// Sirve tanto para cerrar la conexion como el socket
int close_connection(int fd) {
  // TODO
  return close(fd);
}

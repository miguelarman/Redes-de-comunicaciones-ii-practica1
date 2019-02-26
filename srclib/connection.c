#include "../includes/connection.h"


int tcp_listen(char ip, int port) {
  // TODO

  int sockid = socket(AF_INET, SOCK_STREAM, 0);
  if (sockid < 0) {
    // TODO
  }
  // bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
  // listen(int sockfd, int backlog);
  return ERROR;
}

int accept_connection(int sockfd, struct sockaddr* addr, socketlen_t *addrlen) {
  // TODO

  return accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
}

int close_connection(int fd) {
  // TODO

  return close(int fd);
}

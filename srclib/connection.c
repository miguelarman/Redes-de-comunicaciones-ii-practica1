/**
 * @brief Librería que administra las conexiones y sockets
 *
 * En este fichero, se definen las funciones necesarias
 * para manejar una conexión TCP
 *
 * @file connection.c
 * @author Miguel Arconada Manteca y Mario García Pascual
 * @date 9-5-2019
 */

#include "../includes/connection.h"

#define ERROR -1


int tcp_listen(char *ip, int port, int backlog) {
  struct sockaddr_in addr;
  int sockfd;

  /* Inicializamos estructura de dirección y puerto */
  bzero(&addr, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  inet_pton(AF_INET, ip, &(addr.sin_addr));


  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    /* TODO */
  }

  if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    /* TODO */
    perror("Error en el bind");
    return ERROR_BIND;
  }

  if (listen(sockfd, backlog) < 0) {
    /*  TODO */
    perror("Error en el listen");
  }

  return sockfd;
}

int accept_connection(int sockfd, struct sockaddr* addr, socklen_t *addrlen) {
  return accept(sockfd, addr, addrlen);
}

int close_connection(int fd) {
  return close(fd);
}

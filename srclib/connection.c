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

#include <syslog.h>

#define ERROR -1 /*!< Retorno de funciones */


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
    syslog(LOG_ERR, "Error al llamar a socket()");
    return ERROR_SOCKET;
  }

  if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    syslog(LOG_ERR, "Error al llamar a bind()");
    return ERROR_BIND;
  }

  if (listen(sockfd, backlog) < 0) {
    syslog(LOG_ERR, "Error al llamar a listen()");
    return ERROR_LISTEN;
  }

  return sockfd;
}

int accept_connection(int sockfd, struct sockaddr* addr, socklen_t *addrlen) {
  return accept(sockfd, addr, addrlen);
}

int close_connection(int fd) {
  return close(fd);
}

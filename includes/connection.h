#ifndef  CONNECTION_H
#define CONNECTION_H

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

int tcp_listen(char ip, int port);

int accept_connection(int sockfd, struct sockaddr* addr, socketlen_t *addrlen);

int close_connection(int fd);

#endif

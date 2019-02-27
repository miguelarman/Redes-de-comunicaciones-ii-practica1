#ifndef  CONNECTION_H
#define CONNECTION_H

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

int tcp_listen(char *ip, int port, struct sockaddr* addr);

int accept_connection(int sockfd, struct sockaddr* addr, socklen_t *addrlen);

int close_connection(int fd);

#endif

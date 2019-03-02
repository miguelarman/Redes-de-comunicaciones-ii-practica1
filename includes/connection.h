#ifndef  CONNECTION_H
#define CONNECTION_H

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <strings.h>
#include <stdio.h>
#include <errno.h>

#define ERROR_BIND -2


int tcp_listen(char *ip, int port);

int accept_connection(int sockfd, struct sockaddr* addr, socklen_t *addrlen);

int close_connection(int fd);

#endif

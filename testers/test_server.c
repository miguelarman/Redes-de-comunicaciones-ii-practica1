#include "../includes/connection.h"
#include <strings.h>
#include <stdio.h>

#define MAXBUF 1024
#define SERVER_PORT 99
#define SERVER_IP "127.0.0.1"

int main() {
  // char server_addr[] = "127.0.0.1";
  //int port = 99;

  int socketfd;
  int connfd;

  char buffer[MAXBUF];

  struct sockaddr_in self;
  struct sockaddr_in client_addr;
  int addrlen = sizeof(client_addr);



  // Inicializamos estructura de dirección y puerto
	bzero(&self, sizeof(self));
	self.sin_family = AF_INET;
	self.sin_port = htons(SERVER_PORT);
	//self.sin_addr.s_addr = INADDR_ANY;
  inet_aton(SERVER_IP, &self.sin_addr);




  socketfd = tcp_listen(SERVER_IP, SERVER_PORT, (struct sockaddr*) (&self));
  printf("Escuchando en [%s:%d]...\n", inet_ntoa(self.sin_addr), ntohs(self.sin_port));

  while (1) {
    connfd = accept_connection(socketfd, (struct sockaddr*)&client_addr, (socklen_t *)&addrlen);
    printf("Conexión desde [%s:%d]\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    //process_connection(connfd);
    send(connfd, buffer, recv(connfd, buffer, MAXBUF, 0), 0);

  }
  close_connection(connfd);
  close_connection(socketfd);



}

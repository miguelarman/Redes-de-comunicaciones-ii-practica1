#include "../includes/connection.h"
#include "../includes/procesa_peticion.h"
#include <stdio.h>

#define SERVER_PORT 9999
#define SERVER_IP "127.0.0.1"
/* #define SERVER_IP "192.168.1.39" /* Direccion en la red local para probar desde otro dispositivo */

int main() {

  int socketfd;
  int connfd;

  struct sockaddr_in client_addr;
  int addrlen = sizeof(client_addr);

  socketfd = tcp_listen(SERVER_IP, SERVER_PORT);
  printf("Escuchando en [%s:%d]...\n", SERVER_IP, SERVER_PORT);

  while (1) {
    connfd = accept_connection(socketfd, (struct sockaddr*)&client_addr, (socklen_t *)&addrlen);
    printf("Conexión desde [%s:%d]\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    procesa_peticion(connfd);

    close_connection(connfd);
  }
  close_connection(socketfd);



}

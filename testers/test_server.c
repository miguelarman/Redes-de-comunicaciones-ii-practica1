#include "../includes/connection.h"
#include "../includes/procesa_peticion.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/socket.h>


#define SERVER_PORT 9999
#define SERVER_IP "127.0.0.1"
/*#define SERVER_IP "192.168.1.39"*/ /* Direccion en la red local para probar desde otro dispositivo */

void cierre_usuario(int senal);

/* Variables globales */
int socketfd;
int connfd;

int main() {

  struct sockaddr_in client_addr;
  int addrlen = sizeof(client_addr);

  signal(SIGINT, cierre_usuario);

  socketfd = tcp_listen(SERVER_IP, SERVER_PORT);
  if (socketfd == ERROR_BIND) {
    /* TODO */
    printf("Error en tcp_listen\n");
    exit(-1);
  }
  printf("Escuchando en [%s:%d]...\n", SERVER_IP, SERVER_PORT);

  while (1) {
    connfd = accept_connection(socketfd, (struct sockaddr*)&client_addr, (socklen_t *)&addrlen);
    printf("Conexión desde [%s:%d]\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    procesa_peticion(connfd);

    close_connection(connfd);
  }
  close_connection(socketfd);
}

void cierre_usuario(int senal) {
  /* Cierra la conexión y el socket */
  if (close_connection(connfd) < 0) {
    /* TODO */
    perror("Error al cerrar la conexion");
  }

  if (close_connection(socketfd) < 0) {
    /* TODO */
    perror("Error al cerrar el socket");
  }

  /* DEBUG */printf("Saliendo de las conexiones\n");

  exit(EXIT_SUCCESS);
}

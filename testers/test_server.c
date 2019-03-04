#include "../includes/connection.h"
#include "../includes/procesa_conexion.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>



#define SERVER_PORT 9999
#define SERVER_IP "127.0.0.1"
/*#define SERVER_IP "192.168.1.39"*/ /* Direccion en la red local para probar desde otro dispositivo */

void cierre_usuario(int senal);

/* Variables globales */
int socketfd;
int connfd;

int main() {

  char cwd[2048];

  struct sockaddr_in client_addr;
  int addrlen;

  addrlen = sizeof(client_addr);

  signal(SIGINT, cierre_usuario);

  socketfd = tcp_listen(SERVER_IP, SERVER_PORT, 20);
  if (socketfd == ERROR_BIND) {
    /* TODO */
    printf("Error en tcp_listen\n");
    exit(-1);
  }
  printf("Escuchando en [%s:%d]...\n\n", SERVER_IP, SERVER_PORT);

  if (getcwd(cwd, sizeof(cwd)) == NULL) {
    /* TODO */
  }
  strcat(cwd, "/resources");

  int i;
  for (i = 0; i < 3; i++) {
    fork();
  }

  while (1) {
    connfd = accept_connection(socketfd, (struct sockaddr*)&client_addr, (socklen_t *)&addrlen);
    printf("Conexión desde [%s:%d]\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    procesa_conexion(connfd, cwd);

    close_connection(connfd);

    /*break;*/
  }
  close_connection(socketfd);

  return 0;
}

void cierre_usuario(int senal) {
  /* Cierra la conexión y el socket */
  if (connfd >= 0) {
    if (close_connection(connfd) < 0) {
      /* TODO */
      perror("\nError al cerrar la conexion");
    }
  }

  if (socketfd >= 0) {
    if (close_connection(socketfd) < 0) {
      /* TODO */
      perror("\nError al cerrar el socket");
    }
  }

  exit(EXIT_SUCCESS);
}

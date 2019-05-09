#include "../includes/connection.h"
#include "../includes/procesa_conexion.h"
#include "../includes/daemon.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
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
  int i, r;

  /* No hacemos caso a SIGPIPE */
  signal(SIGPIPE, SIG_IGN);

  signal(SIGINT, cierre_usuario);

  if (getcwd(cwd, sizeof(cwd)) == NULL) {
    /* TODO */
  }
  strcat(cwd, "/resources");

  
  r = demonizar();
  syslog(LOG_DEBUG, "Demonizar ha devuelto %d", r);
  if (r == ES_PADRE) {
    exit(EXIT_SUCCESS);
  }

  socketfd = tcp_listen(SERVER_IP, SERVER_PORT, 20);
  if (socketfd == ERROR_BIND) {
    /* TODO */
    syslog(LOG_ERR, "Error en tcp_listen");
    exit(-1);
  }
  syslog(LOG_INFO, "Escuchando en [%s:%d]...", SERVER_IP, SERVER_PORT);



  for (i = 0; i < 3; i++) {
    fork();
  }

  addrlen = sizeof(client_addr);

  while (1) {
    connfd = accept_connection(socketfd, (struct sockaddr*)&client_addr, (socklen_t *)&addrlen);
    syslog(LOG_INFO, "Conexión desde [%s:%d]", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    procesa_conexion(connfd, cwd);

    close_connection(connfd);

    /*break;*/
  }
  close_connection(socketfd);

  wait(NULL);

  exit(EXIT_SUCCESS);
}

void cierre_usuario(int senal) {
  pid_t wpid;
  int status = 0;

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

  /* Espera a todos los hijos */
  while ((wpid = wait(&status)) > 0);

  exit(EXIT_SUCCESS);
}

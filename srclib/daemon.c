#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <syslog.h>
#include <sys/stat.h>

#include "daemon.h"

#define WORKING_DIRECTORY "/"
#define MASK 0
#define OK 0
#define ERROR -1

int demonizar(char *servicio) {

  pid_t child_pid;
  int max_fd, i;
  /*
  Es importante notar que un proceso daemon no está asociado a una terminal, y por tanto no debe utilizar funciones como printf() ni leer o escribir de entrada o salida estándar. Para notificar mensajes, este tipo de procesos utiliza el log del sistema, que típicamente se puede encontrar en /var/log/syslog .
  Funciones a utilizar:
  chdir, close, fork, getdtablesize, open, openlog, setsid, syslog, umask
  Consultar en el manual de la función open el significado de los flags O_RDONLY y O_RDWR
  */

  // Comprobamos el parametro
  if (servicio == NULL) {
    printf("Error con el parametro especificado en demonizar()\n");
    return ERROR;
  }

  // Crear un proceso hijo y terminar el proceso padre
  child_pid = fork();
  if (child_pid < 0) {
    exit(EXIT_FAILURE); // Fallo de creacion del fork
  }
  if (child_pid > 0) {
    //exit(EXIT_SUCCESS);
    return OK; // Salir del padre
  }

  // Abrir el log del sistema para su uso posterior
  setlogmask(LOG_UPTO(LOG_INFO));
  openlog("Server system messages:", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL3);
  syslog(LOG_INFO, "Initiating new daemon.");

  // Crear una nueva sesión de tal forma que el proceso pase a ser el líder de sesión
  if (setsid() < 0) {
    syslog(LOG_ERR, "Error creating a new SID for the child process.");
    exit(EXIT_FAILURE);
  }
  syslog(LOG_INFO, "Daemon is the session leader");


  // Cambiar la máscara de modo de ficheros para que sean accesibles a cualquiera
  umask(MASK);
  syslog(LOG_INFO, "Daemon with new mask");


  // Establecer el directorio raíz / como directorio de trabajo
  if ((chdir(WORKING_DIRECTORY)) < 0) {
    syslog(LOG_ERR, "Error changing the current working directory = \"/\"");
    exit(EXIT_FAILURE);
  }
  syslog(LOG_INFO, "New working directory");


  // Cerrar todos los descriptores de fichero que pueda haber abiertos incluidos stdin, stdout, stderr
  syslog(LOG_INFO, "Closing standard file descriptors");
  max_fd = getdtablesize();
  if (max_fd < 0) {
    syslog(LOG_ERR, "Error closing file descriptors");
    exit(EXIT_FAILURE);
  }

  for (i = 0; i < max_fd; i++) {
    if (close(i) < 0) {
      syslog(LOG_ERR, "Error closing file descriptor %d. It is not open", i);
    }
  }
  syslog(LOG_INFO, "Closed file descriptors");


  return OK;
}

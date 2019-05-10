#include "../includes/daemon.h"

#define WORKING_DIRECTORY "/" /*!< Directorio raíz del sistema */
#define MASK 0                /*!< Valor de la máscara de ficheros */
#define OK 0                  /*!< Retorno de funciones */
#define ERROR -1              /*!< Retorno de funciones */

int demonizar(void) {

  pid_t child_pid;
  int max_fd, i;


  /* Crear un proceso hijo y terminar el proceso padre */
  child_pid = fork();
  if (child_pid < 0) {
    exit(EXIT_FAILURE); /* Fallo de creacion del fork */
  }
  if (child_pid > 0) {
    return ES_PADRE; /* Salir del padre */
  }

  /* Abrir el log del sistema para su uso posterior */
  setlogmask(LOG_UPTO(LOG_INFO));
  openlog("Servidor:", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL3);
  syslog(LOG_INFO, "----------------------------------");
  syslog(LOG_INFO, "Initiating new daemon.");


  /* Crear una nueva sesión de tal forma que el proceso pase a ser el líder de sesión */
  if (setsid() < 0) {
    syslog(LOG_ERR, "Error creating a new SID for the child process.");
    exit(EXIT_FAILURE);
  }
  syslog(LOG_INFO, "Daemon is the session leader");


  /* Cambiar la máscara de modo de ficheros para que sean accesibles a cualquiera */
  umask(MASK);
  syslog(LOG_INFO, "Daemon with new mask");


  /* Establecer el directorio raíz / como directorio de trabajo */
  if ((chdir(WORKING_DIRECTORY)) < 0) {
    syslog(LOG_ERR, "Error changing the current working directory = \"/\"");
    exit(EXIT_FAILURE);
  }
  syslog(LOG_INFO, "New working directory");


  /* Cerrar todos los descriptores de fichero que pueda haber abiertos incluidos stdin, stdout, stderr */
  syslog(LOG_INFO, "Closing standard file descriptors");
  max_fd = getdtablesize();
  if (max_fd < 0) {
    syslog(LOG_ERR, "Error closing file descriptors");
    exit(EXIT_FAILURE);
  }

  for (i = 0; i < max_fd; i++) {
    if (close(i) < 0) {
      /*
      syslog(LOG_ERR, "Error closing file descriptor %d. It is not open", i);
      */
    } else {
      syslog(LOG_INFO, "Succesfully closed file descriptor %d.", i);
    }
  }
  syslog(LOG_INFO, "Closed file descriptors");

  syslog(LOG_INFO, "Ended demon function successfully");

  return OK;
}

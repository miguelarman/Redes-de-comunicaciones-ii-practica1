#ifndef PROCESA_PETICION_H
#define PROCESA_PETICION_H

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sendfile.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include "../includes/picohttpparser.h"

#define PARSEERROR -2
#define REQUESTISTOOLONGERROR -3
#define IOERROR -4
#define CLOSE_CONNECTION_REQUEST -5



typedef struct Parsear_ {
  char buf[4096];
  char *method;
  char *path;
  int pret;
  int minor_version;
  struct phr_header headers[100];
  size_t buflen;
  size_t prevbuflen;
  size_t method_len;
  size_t path_len;
  size_t num_headers;
  ssize_t rret;
} Parsear;

int parsear_peticion(int connfd, Parsear *ret);

int procesa_peticion (int connfd, char *resources_path, Parsear campos_parseados);


#endif

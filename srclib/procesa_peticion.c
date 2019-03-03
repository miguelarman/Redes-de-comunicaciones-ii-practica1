#define _POSIX_C_SOURCE 199309L

#include "../includes/procesa_peticion.h"
#include "../includes/picohttpparser.h"
#include <errno.h>
#include <unistd.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <string.h>
#include <sys/sendfile.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>



#define ERROR -1
#define OK 0

/* TODO Valores a revisar */
#define MAX_FECHA        512
#define MAX_TIPO_FICHERO 32
#define MAX_CABECERA     10000
#define MAX_LINEA        512

#define TAMANIO_CHUNK 1024

#define PARSEERROR -2
#define REQUESTISTOOLONGERROR -3
#define IOERROR -4

#define INDEX_BASICO "media/www/index.html"
#define PAGINA_404   "media/www/404.html"

#define HTTP_RESPONSE_VERSION "HTTP/1.1"

#define RESPONSE_OK_CODE          200
#define RESPONSE_BAD_REQUEST_CODE 400
#define RESPONSE_NOT_FOUND_CODE   404

#define RESPONSE_OK_FRASE          "OK"
#define RESPONSE_BAD_REQUEST_FRASE "Bad Request"
#define RESPONSE_NOT_FOUND_FRASE   "Not Found"

#define HTML_TYPE  "text/html"
#define JPG_TYPE   "image/jpeg"
#define JPEG_TYPE  "image/jpeg"
#define GIF_TYPE   "image/gif"
#define TXT_TYPE   "text/plain"
#define OTHER_TYPE "text/plain"

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

char * _tipo_archivo(char *ruta);

int _cabecera_anadir_fecha_y_hora_actual(char *cabecera_respuesta, int *cabecera_length);

int _cabecera_anadir_tipo_fichero(char *cabecera_respuesta, int *cabecera_length, char *ruta_fichero);

int _cabecera_anadir_ultima_modificacion(char *cabecera_respuesta, int *cabecera_length, char *ruta_fichero);

int _cabecera_anadir_tamanio_fichero(char *cabecera_respuesta, int *cabecera_length, char *ruta_fichero, int *tamanio_fichero);

int _cabecera_terminar(char *cabecera_respuesta, int *cabecera_length);

int _cabecera_anadir_version_html(char *cabecera_respuesta, int *cabecera_length);

int _cabecera_anadir_codigo_respuesta(char *cabecera_respuesta, int *cabecera_length, int codigo);

int _cabecera_anadir_frase_respuesta(char *cabecera_respuesta, int *cabecera_length, char *frase);

int _mandar_fichero_chunks(int connfd, int fichero_a_mandar_df, int tamanio_fichero, int tamanio_chunk, int *bytes_mandados);

int _responder_bad_request (int connfd);

int funcionalidad_get(char *ruta_fichero, int connfd);



int procesa_peticion (int connfd, char *resources_path) {

  char *verbo_peticion      = NULL;
  char *ruta_fichero        = NULL;
  char *cabecera_respuesta  = NULL;
  char *buf;
  char *method;
  char *path;
  int   retorno;
  int   pret;
  int   minor_version;
  struct phr_header *headers;
  Parsear campos_parseados;
  size_t  buflen;
  size_t  prevbuflen;
  size_t  method_len;
  size_t  path_len;
  size_t  num_headers;
  ssize_t rret;

int i;

  retorno = parsear_peticion(connfd, &campos_parseados);
  if (retorno != OK) {
    /* TODO */
  }

  /* Guardo los valores guardados por parsear_peticion en la estructura */
  buf           = campos_parseados.buf;
  method        = campos_parseados.method;
  path          = campos_parseados.path;
  pret          = campos_parseados.pret;
  minor_version = campos_parseados.minor_version;
  headers       = campos_parseados.headers;
  buflen        = campos_parseados.buflen;
  prevbuflen    = campos_parseados.prevbuflen;
  method_len    = campos_parseados.method_len;
  path_len      = campos_parseados.path_len;
  num_headers   = campos_parseados.num_headers;
  rret          = campos_parseados.rret;





  /* DEBUG PRINTEAR LA REQUEST PARA DEBUGGEAR */
  /* printf("request is %d bytes long\n", pret);
  printf("method is %.*s\n", (int)method_len, method);
  printf("path is %.*s\n", (int)path_len, path);
  printf("path is %d bytes long\n", (int)path_len);
  printf("path is %d long\n", (int)path_len);
  printf("HTTP version is 1.%d\n", minor_version);
  printf("headers (%d):\n", (int)num_headers);
  for (i = 0; i != num_headers; ++i) {
    printf("%.*s: %.*s\n", (int)headers[i].name_len, headers[i].name, (int)headers[i].value_len, headers[i].value);
  } */





  if ((verbo_peticion = (char *)malloc(((int)method_len + 1) * sizeof(char))) == NULL) {
    /* TODO */
  }
  if (sprintf(verbo_peticion, "%.*s", (int)method_len, method) < 0) {
    /* TODO */
  }


  if (strcmp(verbo_peticion, "GET") == 0) { /* Petición GET */

    /* Guarda la ruta del fichero */
    ruta_fichero = (char *)malloc(((int)path_len + 1) * sizeof(char));
    if (ruta_fichero == NULL) {
      /* TODO */
    }
    if (sprintf(ruta_fichero, "%.*s", (int)path_len, path) < 0) {
      /* TODO */
    }

    /* TODO Comprueba si es un script */
    /* if (extension(ruta_fichero) == ".py") {

    } else {*/

    retorno = funcionalidad_get(ruta_fichero, connfd);
    if (retorno != OK) {
      /* TODO */
    }

  } else if (strcmp(verbo_peticion, "GET") == 0) { /* Petición GET */

  } else {
    /* TODO Verbo no soportado */
    retorno = _responder_bad_request(connfd);
  }



  free(verbo_peticion);

  free(cabecera_respuesta);


  return ERROR;
}


/**************************************************************/
/**************************************************************/
/**************************************************************/
/**************************************************************/


int parsear_peticion(int connfd, Parsear *campos_a_parsear) {

  campos_a_parsear->buflen = 0;
  campos_a_parsear->prevbuflen = 0;

  while (1) {
    /* read the request */
    while ((campos_a_parsear->rret = read(connfd, campos_a_parsear->buf + campos_a_parsear->buflen, sizeof(campos_a_parsear->buf) - campos_a_parsear->buflen)) == -1
          && errno == EINTR) {

    }

    if (campos_a_parsear->rret <= 0) {
      return IOERROR;
    }

    campos_a_parsear->prevbuflen = campos_a_parsear->buflen;
    campos_a_parsear->buflen += campos_a_parsear->rret;

    /* parse the request */
    campos_a_parsear->num_headers = sizeof(campos_a_parsear->headers) /
                                    sizeof(campos_a_parsear->headers[0]);
    campos_a_parsear->pret = phr_parse_request(
                                      campos_a_parsear->buf,
                                      campos_a_parsear->buflen,
                                      (const char **) &(campos_a_parsear->method),
                                      &(campos_a_parsear->method_len),
                                      (const char **) &(campos_a_parsear->path),
                                      &(campos_a_parsear->path_len),
                                      &(campos_a_parsear->minor_version),
                                      campos_a_parsear->headers,
                                      &(campos_a_parsear->num_headers),
                                      campos_a_parsear->prevbuflen);

    if (campos_a_parsear->pret > 0) {
      break; /* successfully parsed the request */
    } else if (campos_a_parsear->pret == -1) {
      return PARSEERROR;
    }

    /* request is incomplete, continue the loop */
    assert(campos_a_parsear->pret == -2);
    if (campos_a_parsear->buflen == sizeof(campos_a_parsear->buf)) {
      return REQUESTISTOOLONGERROR;
    }
  }

  return OK;
}


char * _tipo_archivo(char *ruta) {

  char *punto = NULL;
  char *extension = NULL;

  if (ruta == NULL) {
    return OTHER_TYPE;
  }


  punto = strrchr(ruta, '.');

  if (punto == NULL || punto == ruta) {
    return OTHER_TYPE;
  } else {
    punto++;
  }

  extension = punto;

  if (strcmp(extension, "html") == 0) {
    return HTML_TYPE;
  } else if (strcmp(extension, "jpg") == 0) {
    return JPG_TYPE;
  } else if (strcmp(extension, "jpeg") == 0) {
    return JPEG_TYPE;
  } else if (strcmp(extension, "gif") == 0) {
    return GIF_TYPE;
  } else if (strcmp(extension, "txt") == 0) {
    return TXT_TYPE;
  } else {
    return OTHER_TYPE;
  }
}

int _cabecera_anadir_fecha_y_hora_actual(char *cabecera_respuesta, int *cabecera_length) {
  time_t now = time(0);
  struct tm tm = *gmtime(&now);
  char *linea_fecha = NULL;

  linea_fecha = (char *)calloc(1, MAX_LINEA * sizeof(char));
  if (linea_fecha == NULL) {
    /* TODO */
  }

  *cabecera_length += strftime(linea_fecha, MAX_FECHA, "Date: %a, %d %b %Y %H:%M:%S %Z\r\n", &tm);
  if (linea_fecha == NULL) {
    /* TODO */
  }
  strcat(cabecera_respuesta, linea_fecha);
  if (cabecera_respuesta == NULL) {
    /* TODO */
  }

  free(linea_fecha);
  return OK;
}

int _cabecera_anadir_tipo_fichero(char *cabecera_respuesta, int *cabecera_length, char *ruta_fichero) {

  char *tipo_fichero = NULL;
  char *linea_tipo = NULL;

  linea_tipo = (char *)calloc(1, MAX_LINEA * sizeof(char));
  if (linea_tipo == NULL) {
    /* TODO */
  }


  tipo_fichero = (char *)calloc(1, MAX_TIPO_FICHERO * sizeof(char));
  if (tipo_fichero == NULL) {
    /* TODO */
  }

  strcpy(tipo_fichero, _tipo_archivo(ruta_fichero));
  if (tipo_fichero == NULL) {
    /* TODO */
  }

  *cabecera_length += sprintf(linea_tipo, "Content-type:%s\r\n", tipo_fichero);
  if (linea_tipo == NULL) {
    /* TODO */
  }
  strcat(cabecera_respuesta, linea_tipo);
  if (cabecera_respuesta == NULL) {
    /* TODO */
  }

  free(tipo_fichero);
  free(linea_tipo);
  return OK;
}

int _cabecera_anadir_ultima_modificacion(char *cabecera_respuesta, int *cabecera_length, char *ruta_fichero) {

  struct stat statbuf;
  struct timespec last_modified;
  struct tm *modificado = NULL;
  char *linea_modificado = NULL;

  linea_modificado = (char *)calloc(1, MAX_LINEA * sizeof(char));
  if (linea_modificado == NULL) {
    /* TODO */
  }

  if (stat(ruta_fichero, &statbuf) < 0) {
    /* TODO */
  }

  last_modified.tv_sec = statbuf.st_mtime;
  modificado = gmtime(&(last_modified.tv_sec));

  *cabecera_length += strftime(linea_modificado, MAX_FECHA, "Last-modified: %a, %d %b %Y %H:%M:%S %Z\r\n", modificado);
  if (linea_modificado == NULL) {
    /* TODO */
  }
  strcat(cabecera_respuesta, linea_modificado);
  if (cabecera_respuesta == NULL) {
    /* TODO */
  }

  free(linea_modificado);
  return OK;
}

int _cabecera_anadir_tamanio_fichero(char *cabecera_respuesta, int *cabecera_length, char *ruta_fichero, int *tamanio_fichero) {

  struct stat statbuf;
  char *linea_tamanio = NULL;

  linea_tamanio = (char *)calloc(1, MAX_LINEA * sizeof(char));
  if (linea_tamanio == NULL) {
    /* TODO */
  }

  if (ruta_fichero == NULL) {
    *tamanio_fichero = 0;
  } else {
    if (stat(ruta_fichero, &statbuf) < 0) {
      /* TODO */
    }
    *tamanio_fichero = statbuf.st_size;
  }

  *cabecera_length += sprintf(linea_tamanio, "Content-length:%d\r\n", *tamanio_fichero);
  if (linea_tamanio == NULL) {
    /* TODO */
  }
  strcat(cabecera_respuesta, linea_tamanio);
  if (cabecera_respuesta == NULL) {
    /* TODO */
  }

  free(linea_tamanio);
  return OK;
}

int _cabecera_terminar(char *cabecera_respuesta, int *cabecera_length) {
  strcat(cabecera_respuesta, "\r\n");
  if (cabecera_respuesta == NULL) {
    /* TODO */
  }
  *cabecera_length += 2;

  return OK;
}

int _cabecera_anadir_version_html(char *cabecera_respuesta, int *cabecera_length) {

  char *linea_version = NULL;

  linea_version = (char *)calloc(1, MAX_LINEA * sizeof(char));
  if (linea_version == NULL) {
    /* TODO */
  }

  *cabecera_length += sprintf(linea_version, "%s ", HTTP_RESPONSE_VERSION);
  if (linea_version == NULL) {
    /* TODO */
  }

  strcat(cabecera_respuesta, linea_version);
  if (cabecera_respuesta == NULL) {
    /* TODO */
  }

  free(linea_version);
  return OK;
}

int _cabecera_anadir_codigo_respuesta(char *cabecera_respuesta, int *cabecera_length, int codigo) {

  char *linea_codigo_respuesta = NULL;

  linea_codigo_respuesta = (char *)calloc(1, MAX_LINEA * sizeof(char));
  if (linea_codigo_respuesta == NULL) {
    /* TODO */
  }

  *cabecera_length += sprintf(linea_codigo_respuesta, "%d ", codigo);
  if (linea_codigo_respuesta == NULL) {
    /* TODO */
  }

  strcat(cabecera_respuesta, linea_codigo_respuesta);
  if (cabecera_respuesta == NULL) {
    /* TODO */
  }

  free(linea_codigo_respuesta);
  return OK;
}

int _cabecera_anadir_frase_respuesta(char *cabecera_respuesta, int *cabecera_length, char *frase) {

  char *linea_frase_respuesta = NULL;

  linea_frase_respuesta = (char *)calloc(1, MAX_LINEA * sizeof(char));
  if (linea_frase_respuesta == NULL) {
    /* TODO */
  }

  *cabecera_length += sprintf(linea_frase_respuesta, "%s\r\n", frase);
  if (linea_frase_respuesta == NULL) {
    /* TODO */
  }

  strcat(cabecera_respuesta, linea_frase_respuesta);
  if (cabecera_respuesta == NULL) {
    /* TODO */
  }

  free(linea_frase_respuesta);
  return OK;
}

int _mandar_fichero_chunks(int connfd, int fichero_a_mandar_df, int tamanio_fichero, int tamanio_chunk, int *bytes_mandados) {

  off_t *offset = NULL;
  int mandados = 0;

  while (mandados < tamanio_fichero) {
    mandados += sendfile(connfd, fichero_a_mandar_df, offset, tamanio_chunk);
    if (mandados < 0) {
      /* TODO */
    }
  }

  *bytes_mandados = mandados;

  return OK;
}

int _responder_bad_request (int connfd) {

  char *cabecera_respuesta;
  int cabecera_length = 0;
  int retorno;

  cabecera_respuesta = (char *)calloc(1, MAX_CABECERA * sizeof(char));
  if (cabecera_respuesta == NULL) {
    /* TODO */
  }

  /* 1.1 Escribe la version */
  retorno = _cabecera_anadir_version_html(cabecera_respuesta, &cabecera_length);
  if (retorno != OK) {
    /* TODO */
  }

  /* 1.2 Escribe el codigo de respuesta */
  retorno = _cabecera_anadir_codigo_respuesta(cabecera_respuesta, &cabecera_length, RESPONSE_BAD_REQUEST_CODE);
  if (retorno != OK) {
    /* TODO */
  }

  /* 1.3 Escribe la frase de respuesta */
  retorno = _cabecera_anadir_frase_respuesta(cabecera_respuesta, &cabecera_length, RESPONSE_BAD_REQUEST_FRASE);
  if (retorno != OK) {
    /* TODO */
  }

  /* 2. Escribe los campos de cabecera */

  /* 2.1 Escribe el tipo de fichero */
  retorno = _cabecera_anadir_tipo_fichero(cabecera_respuesta, &cabecera_length, NULL);
  if (retorno != OK) {
    /* TODO */
  }

  /* 2.2 Escribe el tamaño de fichero */
  retorno = _cabecera_anadir_tamanio_fichero(cabecera_respuesta, &cabecera_length, NULL, NULL);
  if (retorno != OK) {
    /* TODO */
  }

  /* 2.3. Escribe la fecha y hora actuales */
  retorno = _cabecera_anadir_fecha_y_hora_actual(cabecera_respuesta, &cabecera_length);
  if (retorno != OK) {
    /* TODO */
  }

  /* 2.5 Termina la cabecera */
  retorno = _cabecera_terminar(cabecera_respuesta, &cabecera_length);
  if (retorno != OK) {
    /* TODO */
  }

  /* TODO 3. Manda la cabecera */
  if (send(connfd, cabecera_respuesta, cabecera_length, 0) < 0) {
    /* TODO */
  }

  free(cabecera_respuesta);
  return OK;
}


int funcionalidad_get(char *ruta_fichero, int connfd) {

  int   bytes_mandados      =  0;
  int   tamanio_fichero     =  0;
  int   cabecera_length     =  0;
  int   path_len            =  0;
  int   fichero_a_mandar_df = -1;
  int   retorno             =  ERROR;
  char *cabecera_respuesta  = NULL;

  if (strcmp(ruta_fichero, "/") == 0) {
    /* Responder html básico */
    free(ruta_fichero);
    path_len = strlen(INDEX_BASICO);
    ruta_fichero = (char *)malloc((path_len + 1) * sizeof(char));
    strcpy(ruta_fichero, INDEX_BASICO);

  }
  if (ruta_fichero[0] == '/') {
    /* Eliminar el / del principio de la cadena */
    char *nueva_ruta_fichero = (char *)malloc(strlen(ruta_fichero) * sizeof(char));
    strcpy(nueva_ruta_fichero, ruta_fichero + 1);

    free(ruta_fichero);
    ruta_fichero = nueva_ruta_fichero;
  }



  cabecera_respuesta = (char *)calloc(1, MAX_CABECERA * sizeof(char));
  if (cabecera_respuesta == NULL) {
    /* TODO */
  }


  /* Abre el fichero pedido */
  fichero_a_mandar_df = open(ruta_fichero, O_RDONLY);


  /* Forma la cabecera */

  /* 1.1 Escribe la version */
  retorno = _cabecera_anadir_version_html(cabecera_respuesta, &cabecera_length);
  if (retorno != OK) {
    /* TODO */
  }

  if (fichero_a_mandar_df < 0) { /* El fichero pedido no existe */

    /* 1.2 Escribe el codigo de respuesta */
    retorno = _cabecera_anadir_codigo_respuesta(cabecera_respuesta, &cabecera_length, RESPONSE_NOT_FOUND_CODE);
    if (retorno != OK) {
      /* TODO */
    }

    /* 1.3 Escribe la frase de respuesta */
    retorno = _cabecera_anadir_frase_respuesta(cabecera_respuesta, &cabecera_length, RESPONSE_NOT_FOUND_FRASE);
    if (retorno != OK) {
      /* TODO */
    }

    /* Actualiza los valores con la página de 404 para los siguientes valores de la cabecera */
    free(ruta_fichero);
    path_len = strlen(PAGINA_404);
    ruta_fichero = (char *)malloc(((int)path_len + 1) * sizeof(char));
    strcpy(ruta_fichero, PAGINA_404);

    fichero_a_mandar_df = open(ruta_fichero, O_RDONLY);
    if (fichero_a_mandar_df < 0) {
      /* TODO */
    }

  } else {

    /* 1.2 Escribe el codigo de respuesta */
    retorno = _cabecera_anadir_codigo_respuesta(cabecera_respuesta, &cabecera_length, RESPONSE_OK_CODE);
    if (retorno != OK) {
      /* TODO */
    }

    /* 1.3 Escribe la frase de respuesta */
    retorno = _cabecera_anadir_frase_respuesta(cabecera_respuesta, &cabecera_length, RESPONSE_OK_FRASE);
    if (retorno != OK) {
      /* TODO */
    }
  }

  /* 2. Escribe los campos de cabecera */

  /* 2.1 Escribe el tipo de fichero */
  retorno = _cabecera_anadir_tipo_fichero(cabecera_respuesta, &cabecera_length, ruta_fichero);
  if (retorno != OK) {
    /* TODO */
  }

  /* 2.2 Escribe el tamaño de fichero */
  retorno = _cabecera_anadir_tamanio_fichero(cabecera_respuesta, &cabecera_length, ruta_fichero, &tamanio_fichero);
  if (retorno != OK) {
    /* TODO */
  }

  /* 2.3. Escribe la fecha y hora actuales */
  retorno = _cabecera_anadir_fecha_y_hora_actual(cabecera_respuesta, &cabecera_length);
  if (retorno != OK) {
    /* TODO */
  }

  if (fichero_a_mandar_df >= 0) { /* Se ha pedido un archivo que existe */

    /* 2.4. Escribe la hora de ultima modificacion del fichero */
    retorno = _cabecera_anadir_ultima_modificacion(cabecera_respuesta, &cabecera_length, ruta_fichero);
    if (retorno != OK) {
      /* TODO */
    }
  }

  /* 2.5 Termina la cabecera */
  retorno = _cabecera_terminar(cabecera_respuesta, &cabecera_length);
  if (retorno != OK) {
    /* TODO */
  }


  /* TODO 3. Manda la cabecera */
  if (send(connfd, cabecera_respuesta, cabecera_length, 0) < 0) {
    /* TODO */
  }

  /* 4. Manda el archivo */
  retorno = _mandar_fichero_chunks(connfd, fichero_a_mandar_df, tamanio_fichero, TAMANIO_CHUNK, &bytes_mandados);

  if (bytes_mandados < 0) {
    /* TODO */
  } else {
    /* DEBUG */
    printf("Archivo mandado (%d bytes)\n", bytes_mandados);
  }

  /* DEBUG */
  printf("----------------------\n");


  if (fichero_a_mandar_df > 0) {
    close(fichero_a_mandar_df);
  }

  free(ruta_fichero);
  free(cabecera_respuesta);

  return OK;
}

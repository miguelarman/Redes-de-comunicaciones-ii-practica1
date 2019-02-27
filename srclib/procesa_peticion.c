#include "../includes/procesa_peticion.h"
#include "../includes/picohttpparser.h"
#include <errno.h>
#include <unistd.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <sys/sendfile.h>
#include <fcntl.h>
#include <time.h>


#define ERROR -1
#define MAXBUF 1024
#define MAX_FECHA 1024

#define PARSEERROR -2
#define REQUESTISTOOLONGERROR -3
#define IOERROR -4
/* #define INDEX_BASICO "www/index_prueba.html" */
#define INDEX_BASICO "/www/index.html"

#define HTTP_RESPONSE_VERSION "HTTP/1.1"

#define RESPONSE_OK_CODE 200
#define RESPONSE_NOT_FOUND_CODE 404

#define RESPONSE_OK_FRASE "OK"
#define RESPONSE_NOT_FOUND_FRASE "Not Found"

#define HTML_EXTENSION 3
#define JPG_EXTENSION  4
#define JPEG_EXTENSION 5
#define GIF_EXTENSION  6
#define TXT_EXTENSION  7
#define OTHER_EXTENSION  8

#define HTML_TYPE "text/html"
#define JPG_TYPE "image/jpg"
#define JPEG_TYPE "image/jpeg"
#define GIF_TYPE "image/gif"
#define TXT_TYPE "text/plain"
#define CHARSET "charset=UTF-8"

#define MAX_RESPUESTA 10000 /* TODO Valores a revisar */
#define MAX_LINEA 1000

int _extension_archivo(char *ruta);
char * _tipo_archivo(char *ruta);

int procesa_peticion (int connfd) {

  char buf[4096], *method, *path;
  int pret, minor_version;
  struct phr_header headers[100];
  size_t buflen = 0, prevbuflen = 0, method_len, path_len, num_headers;
  ssize_t rret;
  int i;

  /* char buffer[MAXBUF];

  size_t size = recv(connfd, buffer, MAXBUF, 0);

  send(connfd, buffer, size, 0); */

  while (1) {
    /* read the request */
    while ((rret = read(connfd, buf + buflen, sizeof(buf) - buflen)) == -1 && errno == EINTR) {

    }

    if (rret <= 0) {
      return IOERROR;
    }

    prevbuflen = buflen;
    buflen += rret;

    /* parse the request */
    num_headers = sizeof(headers) / sizeof(headers[0]);
    pret = phr_parse_request(buf, buflen, (const char **) &method, &method_len,
                             (const char **) &path, &path_len, &minor_version,
                             headers, &num_headers, prevbuflen);
    if (pret > 0) {
      break; /* successfully parsed the request */
    } else if (pret == -1) {
      return PARSEERROR;
    }

    /* request is incomplete, continue the loop */
    assert(pret == -2);
    if (buflen == sizeof(buf)) {
      return REQUESTISTOOLONGERROR;
    }
  }

  /* PRINTEAR LA REQUEST PARA DEBUGGEAR */
  printf("request is %d bytes long\n", pret);
  printf("method is %.*s\n", (int)method_len, method);
  printf("path is %.*s\n", (int)path_len, path);
  printf("HTTP version is 1.%d\n", minor_version);
  printf("headers:\n");
  for (i = 0; i != num_headers; ++i) {
    printf("%.*s: %.*s\n", (int)headers[i].name_len, headers[i].name, (int)headers[i].value_len, headers[i].value);
  }

  char *verbo_peticion     = NULL;
  char *ruta_fichero       = NULL;
  char *cabecera_respuesta = NULL;
  char *linea_campo        = NULL; /* TODO no hacer tantos callocs */
  char *tipo_fichero       = NULL;
  FILE *fichero_a_mandar   = NULL;
  int fichero_a_mandar_df = -1;
  int   cabecera_length    = 0;
  int   tamanio_fichero    = 0;
  struct stat statbuf;


  if ((verbo_peticion = (char *)malloc(((int)method_len + 1) * sizeof(char))) == NULL) {
    /* TODO */
  }
  if (sprintf(verbo_peticion, "%.*s", (int)method_len, method) < 0) {
    /* TODO */
  }


  /* DEBUG */
  printf("---------------------------\nMetodo: %s\n", verbo_peticion);

  if (strcmp(verbo_peticion, "GET") == 0) { /* Petición GET */
    /* Guarda la ruta del fichero */
    if ((ruta_fichero = (char *)malloc(((int)path_len + 1) * sizeof(char))) == NULL) {
      /* TODO */
    }
    if (sprintf(ruta_fichero, "%.*s", (int)path_len, path) < 0) {
      /* TODO */
    }

    if (strcmp(ruta_fichero, "/") == 0) {
      /* Responder html básico */
      strcpy(ruta_fichero, INDEX_BASICO);
    }
    if (ruta_fichero[0] == '/') {
      ruta_fichero++;
    }

    /* DEBUG */printf("--------------\n");

    cabecera_respuesta = (char *)calloc(1, MAX_RESPUESTA * sizeof(char));
    if (cabecera_respuesta == NULL) {
      /* TODO */
      /* DEBUG */printf("Error con el calloc de cabecera_respuesta\n");
    }


    /* TODO */
    /* Abre el fichero pedido */
    /* fichero_a_mandar = fopen(ruta_fichero, "r"); */
    /* DEBUG */printf("Va a abrir el fichero\n");
    fichero_a_mandar_df = open(ruta_fichero, O_RDONLY);
    /* DEBUG */printf("Llama a open: %d\n", fichero_a_mandar_df);


    /* Forma todos los campos de la cabecera */
    /* TODO rehacer con sprintf? */

    /* 1- Escribe la linea de respuesta */

    /* 1.1 Escribe la version */
    linea_campo = (char *)calloc(1, MAX_LINEA * sizeof(char));
    if (linea_campo == NULL) {
      /* TODO */
    }

    cabecera_length += sprintf(linea_campo, "%s ", HTTP_RESPONSE_VERSION);
    if (linea_campo == NULL) {
      /* TODO */
    }

    strcat(cabecera_respuesta, linea_campo);
    if (cabecera_respuesta == NULL) {
      /* TODO */
      /* DEBUG */ printf("Un strcat da NULL\n");
    }


    /* if (fichero_a_mandar == NULL) { /* El fichero pedido no existe */
    if (fichero_a_mandar_df < 0) { /* El fichero pedido no existe */
      /* DEBUG */printf("Fichero pedido no existe: %s\n", ruta_fichero);


      /* 1.2 Escribe el codigo de respuesta */
      free(linea_campo);
      linea_campo = (char *)calloc(1, MAX_LINEA * sizeof(char));
      if (linea_campo == NULL) {
        /* TODO */
      }

      cabecera_length += sprintf(linea_campo, "%d ", RESPONSE_NOT_FOUND_CODE);
      if (linea_campo == NULL) {
        /* TODO */
      }

      strcat(cabecera_respuesta, linea_campo);
      if (cabecera_respuesta == NULL) {
        /* TODO */
        /* DEBUG */ printf("Un strcat da NULL\n");
      }

      /* 1.3 Escribe la frase de respuesta */
      free(linea_campo);
      linea_campo = (char *)calloc(1, MAX_LINEA * sizeof(char));
      if (linea_campo == NULL) {
        /* TODO */
      }

      cabecera_length += sprintf(linea_campo, "%s\r\n", RESPONSE_NOT_FOUND_FRASE);
      if (linea_campo == NULL) {
        /* TODO */
      }

      strcat(cabecera_respuesta, linea_campo);
      if (cabecera_respuesta == NULL) {
        /* TODO */
        /* DEBUG */ printf("Un strcat da NULL\n");
      }

      strcat(cabecera_respuesta, "\r\n");
      cabecera_length += 4;
      if (cabecera_respuesta == NULL) {
        /* TODO */
      }

      tamanio_fichero = 0;

      free(linea_campo);

    } else { /* Se ha pedido un archivo que existe */
      /* DEBUG */printf("Fichero pedido: %s\n", ruta_fichero);

      /* 1.2 Escribe el codigo de respuesta */
      free(linea_campo);
      linea_campo = (char *)calloc(1, MAX_LINEA * sizeof(char));
      if (linea_campo == NULL) {
        /* TODO */
      }

      cabecera_length += sprintf(linea_campo, "%d ", RESPONSE_OK_CODE);
      if (linea_campo == NULL) {
        /* TODO */
      }

      strcat(cabecera_respuesta, linea_campo);
      if (cabecera_respuesta == NULL) {
        /* TODO */
        /* DEBUG */ printf("Un strcat da NULL\n");
      }

      /* 1.3 Escribe la frase de respuesta */
      free(linea_campo);
      linea_campo = (char *)calloc(1, MAX_LINEA * sizeof(char));
      if (linea_campo == NULL) {
        /* TODO */
      }

      cabecera_length += sprintf(linea_campo, "%s\r\n", RESPONSE_OK_FRASE);
      if (linea_campo == NULL) {
        /* TODO */
      }

      strcat(cabecera_respuesta, linea_campo);
      if (cabecera_respuesta == NULL) {
        /* TODO */
        /* DEBUG */ printf("Un strcat da NULL\n");
      }

      /* 2. Escribe los campos de cabecera */

      /* 2.1 Escribe el tipo de fichero */
      tipo_fichero = _tipo_archivo(ruta_fichero);
      if (tipo_fichero == NULL) {
        /* TODO */
      }
      free(linea_campo);
      linea_campo = (char *)calloc(1, MAX_LINEA * sizeof(char));
      if (linea_campo == NULL) {
        /* TODO */
      }

      cabecera_length += sprintf(linea_campo, "Content-type:%s\r\n", tipo_fichero);
      if (linea_campo == NULL) {
        /* TODO */
      }
      strcat(cabecera_respuesta, linea_campo);
      if (cabecera_respuesta == NULL) {
        /* TODO */
      }

      /* 2.2 Escribe el tamaño de fichero */
      if (stat(ruta_fichero, &statbuf) < 0) {
        /* TODO */
      }
      tamanio_fichero = statbuf.st_size;

      free(linea_campo);
      linea_campo = (char *)calloc(1, MAX_LINEA * sizeof(char));
      if (linea_campo == NULL) {
        /* TODO */
      }

      cabecera_length += sprintf(linea_campo, "Content-length:%d\r\n", tamanio_fichero);
      if (linea_campo == NULL) {
        /* TODO */
      }
      strcat(cabecera_respuesta, linea_campo);
      if (cabecera_respuesta == NULL) {
        /* TODO */
      }

      /* TODO 2.3. Escribe la fecha y hora actuales */
      time_t now = time(0);
      struct tm tm = *gmtime(&now);
      free(linea_campo);
      linea_campo = (char *)calloc(1, MAX_LINEA * sizeof(char));
      if (linea_campo == NULL) {
        /* TODO */
      }

      /* cabecera_length += sprintf(linea_campo, "Date:%s\r\n", actual); */
      cabecera_length += strftime(linea_campo, MAX_FECHA, "Date: %a, %d %b %Y %H:%M:%S %Z\r\n", &tm);
      /* DEBUG */printf("Linea con la fecha: %s\n", linea_campo);
      if (linea_campo == NULL) {
        /* TODO */
      }
      strcat(cabecera_respuesta, linea_campo);
      if (cabecera_respuesta == NULL) {
        /* TODO */
      }

      /* TODO 2.4. Escribe la ultima modificacion */
      /*struct timespec last_modified = statbuf.st_mtime;
      free(linea_campo);
      linea_campo = (char *)calloc(1, MAX_LINEA * sizeof(char));
      if (linea_campo == NULL) {
        /* TODO */
      /*}

      cabecera_length += sprintf(linea_campo, "Last-modified:%s\r\n", last_modified);
      if (linea_campo == NULL) {
        /* TODO */
      /*}
      strcat(cabecera_respuesta, linea_campo);
      if (cabecera_respuesta == NULL) {
        /* TODO */
      /*}*/




      strcat(cabecera_respuesta, "\r\n");
      cabecera_length += 4;
      if (cabecera_respuesta == NULL) {
        /* TODO */
      }

      free(linea_campo);
    }

    /* DEBUG */
    printf("Cabecera:\n%s\n", cabecera_respuesta);
    /* TODO 3. Manda la cabecera */
    if (send(connfd, cabecera_respuesta, cabecera_length, 0) < 0) {
      /* TODO */
    }
    /* DEBUG */printf("Cabecera mandada\n");

    /* DEBUG */printf("Va a mandar fichero de %d bytes\n", tamanio_fichero);
    /* TODO 4. Manda el archivo */
    /*if (send(connfd, fichero_a_mandar, tamanio_fichero, 0) < 0) {
      /* TODO */
    /*}*/
    int bytes_mandados = sendfile(connfd, fichero_a_mandar_df, NULL, tamanio_fichero);
    if (bytes_mandados < 0) {
      /* TODO */

      /* DEBUG */
      printf("Error con sendfile\n");
    } else {
      /* DEBUG */printf("Archivo mandado (%d bytes)\n", bytes_mandados);
    }

    /* DEBUG */printf("----------------------\n\n\n\n");


    if (fichero_a_mandar_df > 0) {
      close(fichero_a_mandar_df);
    }

    ruta_fichero--;
    free(ruta_fichero);

  } else {
    /* TODO Verbo no soportado */
  }

  free(verbo_peticion);

  free(cabecera_respuesta);


  return ERROR;
}


/**************************************************************/
/**************************************************************/
/**************************************************************/
/**************************************************************/

int _extension_archivo(char *ruta) {
  const char *punto = strrchr(ruta, '.');

  if (punto == NULL || punto == ruta) {
    punto = NULL;
  } else {
    punto ++;
  }

  if (strcmp(punto, "html") == 0) {
    return HTML_EXTENSION;
  } else if (strcmp(punto, "jpg") == 0) {
    return JPG_EXTENSION;
  } else if (strcmp(punto, "jpeg") == 0) {
    return JPEG_EXTENSION;
  } else if (strcmp(punto, "gif") == 0) {
    return GIF_EXTENSION;
  } else if (strcmp(punto, "txt") == 0) {
    return TXT_EXTENSION;
  } else {
    return OTHER_EXTENSION;
  }


}
char * _tipo_archivo(char *ruta) {
  if (_extension_archivo(ruta) == HTML_EXTENSION) {
    return HTML_TYPE;
  } else if (_extension_archivo(ruta) == JPEG_EXTENSION) {
    return JPEG_TYPE;
  } else if (_extension_archivo(ruta) == JPG_EXTENSION) {
    return JPG_TYPE;
  } else if (_extension_archivo(ruta) == GIF_EXTENSION) {
    return GIF_TYPE;
  } else if (_extension_archivo(ruta) == TXT_EXTENSION) {
    return TXT_TYPE;
  } else {

  }
}

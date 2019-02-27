#include "../includes/procesa_peticion.h"
#include "../includes/picohttpparser.h"
#include <errno.h>
#include <unistd.h>
#include <assert.h>

#define ERROR -1
#define MAXBUF 1024

#define PARSEERROR -2
#define REQUESTISTOOLONGERROR -3
#define IOERROR -4


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

  char *verbo_peticion = NULL;
  char *ruta = NULL;

  if ((verbo_peticion = (char *)malloc((int)method_len * sizeof(char))) == NULL) {
    /* TODO */
  }
  if (sprintf(verbo_peticion, "%.*s", (int)method_len, method) < 0) {
    /* TODO */
  }


  /* DEBUG */
  printf("---------------------------\nMetodo: %s\n", verbo_peticion);

  if (strcmp(verbo_peticion, "GET") == 0) { /* Petición GET */
    if ((ruta = (char *)malloc((int)path_len * sizeof(char))) == NULL) {
      /* TODO */
    }
    if (sprintf(ruta, "%.*s", (int)path_len, path) < 0) {
      /* TODO */
    }
    /* DEBUG */
    printf("Ruta: %s\n", ruta);

    if (strcmp(ruta, "/") == 0) {
      /* Responder html básico */
      strcpy(ruta, "index_prueba.html");
    }
    char *cabecera_respuesta = NULL;

    cabecera_respuesta = (char *)calloc(MAX_RESPUESTA * sizeof(char));
    if (cabecera_respuesta == NULL) {
      /* TODO */
    }
    /* TODO */
    /* Forma todos los campos de la cabecera*/

    /* TODO*/
    /* Manda la cabecera */
    /* Envía el fichero */
  
  } else {
    /* Verbo no soportado */
  }


  /**************************

  char *verbo_peticion = NULL;
  char *ruta = NULL;

  // Esperar a una petición

  // Leer la petición
  verbo_peticion = -------------;
  if (verbo_peticion == NULL) {
    // Error al leer la petición
  }

   if (strcpm(verbo_peticion, GET) == 0) {
     // Recibe una petición GET

     // Lee el archivo que se le pide
     ruta = -------------;
     if (ruta == NULL) {
       // Error al leer la petición
     }

     if (strcmp(ruta, "/") == 0) {
       // Devuelve el index.html base
     } else if (strcmp(ruta, ruta_otro_fichero) == 0) {
       // Devuelve el fichero específico
       // Se usa un buffer de tamaño fijo (1 KB)
       // y se envía "a trozos" el fichero
     } else {
       // Fichero no conocido
       // Devuelve un mensaje de error 404
     }
   } else if (strcpm(verbo_peticion, POST) == 0) {
     // Recibe un POST
   } else if (strcpm(verbo_peticion, OPTIONS) == 0) {
     // Recibe una petición OPTIONS
   } else {
     // Verbo de petición no soportada
   }

   */






  return ERROR;
}

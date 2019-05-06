#define _POSIX_C_SOURCE 199309L

#include "../includes/procesa_peticion.h"


#define ERROR -1
#define OK 0
#define TRUE 1
#define FALSE 0

/* TODO Valores a revisar */
#define MAX_FECHA        512
#define MAX_TIPO_FICHERO 32
#define MAX_CABECERA     10000
#define MAX_LINEA        512

#define TAMANIO_CHUNK 1024

#define INDEX_BASICO "/www/index.html"
#define PAGINA_404   "/www/404.html"

#define HTTP_RESPONSE_VERSION "HTTP/1.1"

#define ALLOW_LISTA "OPTIONS, GET, POST"

#define RESPONSE_OK_CODE          200
#define RESPONSE_BAD_REQUEST_CODE 400
#define RESPONSE_NOT_FOUND_CODE   404

#define RESPONSE_OK_FRASE          "OK"
#define RESPONSE_BAD_REQUEST_FRASE "Bad Request"
#define RESPONSE_NOT_FOUND_FRASE   "Not Found"

#define HTML_TYPE  "text/html"
#define JPG_TYPE   "image/jpeg"
#define JPEG_TYPE  "image/jpeg"
#define ICO_TYPE   "image/x-icon"
#define GIF_TYPE   "image/gif"
#define TXT_TYPE   "text/plain"
#define MPEG_TYPE  "video/mpeg"
#define MP4_TYPE   "video/mp4"
#define PDF_TYPE   "application/pdf"
#define DOC_TYPE   "application/msword"
#define DOCX_TYPE  "application/msword"
#define WAV_TYPE   "audio/wav"
#define OTHER_TYPE "text/plain"

#define EXTENSION_HTML   ".html"
#define EXTENSION_JPG    ".jpg"
#define EXTENSION_JPEG   ".jpeg"
#define EXTENSION_ICO    ".ico"
#define EXTENSION_GIF    ".gif"
#define EXTENSION_TXT    ".txt"
#define EXTENSION_MPEG   ".mpeg"
#define EXTENSION_MP4    ".mp4"
#define EXTENSION_PDF    ".pdf"
#define EXTENSION_DOC    ".doc"
#define EXTENSION_DOCX   ".docx"
#define EXTENSION_WAV    ".wav"
#define EXTENSION_PYTHON ".py"
#define EXTENSION_PHP    ".php"


int funcionalidad_get(char *ruta_fichero, char *resources_path, int connfd);

int funcionalidad_options(int connfd, char *resources_path);

char * _tipo_archivo(char *ruta);

int ejecutar_script(int connfd, char* resources_path, char *ruta_absoluta);

int _cabecera_anadir_fecha_y_hora_actual(char *cabecera_respuesta, int *cabecera_length);

int _cabecera_anadir_tipo_fichero(char *cabecera_respuesta, int *cabecera_length, char *ruta_fichero);

int _cabecera_anadir_ultima_modificacion(char *cabecera_respuesta, int *cabecera_length, char *ruta_fichero);

int _cabecera_anadir_allow(char *cabecera_respuesta, int *cabecera_length);

int _cabecera_anadir_tamanio_fichero(char *cabecera_respuesta, int *cabecera_length, char *ruta_fichero, int *tamanio_fichero);

int _cabecera_terminar(char *cabecera_respuesta, int *cabecera_length);

int _cabecera_anadir_version_html(char *cabecera_respuesta, int *cabecera_length);

int _cabecera_anadir_codigo_respuesta(char *cabecera_respuesta, int *cabecera_length, int codigo);

int _cabecera_anadir_frase_respuesta(char *cabecera_respuesta, int *cabecera_length, char *frase);

int _mandar_fichero_chunks(int connfd, int fichero_a_mandar_df, int tamanio_fichero, int tamanio_chunk, int *bytes_mandados);

int _responder_bad_request (int connfd, char* resources_path);

int _responder_404 (int connfd, char *resources_path);

int _responder_404_sin_ficheros (int connfd);

int _manda_respuesta_con_fichero(int connfd, int codigo, char *frase, char *ruta_fichero, int fichero_a_mandar_df);

int _manda_respuesta_sin_fichero(int connfd, int codigo, char *frase);

int _fichero_es_script(char *ruta);

int get_ruta_script_y_variables(char *ruta_absoluta, char **ruta_script_p, char **variables_p);



int procesa_peticion (int connfd, char *resources_path, Parsear campos_parseados) {

  char *verbo_peticion      = NULL;
  char *ruta_fichero        = NULL;
  char *cabecera_respuesta  = NULL;
  /*
  char *buf                 = NULL;
  */
  char *method              = NULL;
  char *path                = NULL;
  int   retorno;
  /*
  int   pret;
  int   minor_version;
  struct phr_header *headers;
  size_t  buflen;
  size_t  prevbuflen;
  */
  size_t  method_len;
  size_t  path_len;
  /*
  size_t  num_headers;
  ssize_t rret;

  int i;
  */

  /*if (campos_parseados == NULL) {
    return ERROR; *//* TODO */
  /*}*/



  /* Guardo los valores guardados por parsear_peticion en la estructura */
  /*buf           = campos_parseados.buf;*/
  method        = campos_parseados.method;
  path          = campos_parseados.path;
  /*
  pret          = campos_parseados.pret;
  minor_version = campos_parseados.minor_version;
  headers       = campos_parseados.headers;
  buflen        = campos_parseados.buflen;
  prevbuflen    = campos_parseados.prevbuflen;
  */
  method_len    = campos_parseados.method_len;
  path_len      = campos_parseados.path_len;
  /*
  num_headers   = campos_parseados.num_headers;
  rret          = campos_parseados.rret;
  */

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

  /* Comprobaciones de argumentos */
  if (resources_path == NULL) {
    /* TODO */
  }
  if (connfd < 0) {
    /* TODO */
  }


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

    retorno = funcionalidad_get(ruta_fichero, resources_path, connfd);
    if (retorno != OK) {
      /* TODO */
    }

  } else if (strcmp(verbo_peticion, "OPTIONS") == 0) { /* Petición OPTIONS */

    retorno = funcionalidad_options(connfd, resources_path);
    if (retorno != OK) {
      /* TODO */
    }

  } else if (strcmp(verbo_peticion, "POST") == 0) { /* Petición POST */

    retorno = _responder_bad_request(connfd, resources_path);
    if (retorno != OK) {
      /* TODO */
    }

  } else {

    /* TODO Verbo no soportado */
    retorno = _responder_bad_request(connfd, resources_path);
    if (retorno != OK) {
      /* TODO */
    }
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

    /* campos_a_parsear->rret = recv(connfd, campos_a_parsear->buf, 4096, 0); */

    /* DEBUG *//*printf("Ha salido del bucle de read (%d bytes)\n", (int)campos_a_parsear->rret);*/

    /*if (errno == EINTR) {
      *//* DEBUG *//*printf("Ha recibido el timeout\n");
      return CLOSE_CONNECTION_REQUEST;
    }*/

    if (campos_a_parsear->rret < 0) {
      return IOERROR;
    } else if (campos_a_parsear->rret == 0) {
      return CLOSE_CONNECTION_REQUEST;
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

int funcionalidad_get(char *ruta_fichero, char *resources_path, int connfd) {

  int   path_len            =  0;
  int   fichero_a_mandar_df = -1;
  int   retorno             =  ERROR;
  char *ruta_absoluta       = NULL;

  /* Guarda la ruta relativa del fichero pedido */
  if (strcmp(ruta_fichero, "/") == 0) {
    /* Responder html básico */
    free(ruta_fichero);
    path_len = strlen(INDEX_BASICO);
    ruta_fichero = (char *)malloc((path_len + 1) * sizeof(char));
    strcpy(ruta_fichero, INDEX_BASICO);
  }

  ruta_absoluta = (char *)malloc((strlen(ruta_fichero) + strlen(resources_path) + 1) * sizeof(char));
  if (ruta_absoluta == NULL) {
    /* TODO */
  }

  if (sprintf(ruta_absoluta, "%s%s", resources_path, ruta_fichero) < 0) {
    /* TODO */
  }

  /* TODO Peticion GET a un script */
  if (_fichero_es_script(ruta_absoluta) == TRUE) {
    retorno = ejecutar_script(connfd, resources_path, ruta_absoluta);
    if (retorno != OK) {
      /* TODO */
    }

  } else { /* Se ha solicitado un fichero */

    /* Abre el fichero pedido */
    fichero_a_mandar_df = open(ruta_absoluta, O_RDONLY);

    if (fichero_a_mandar_df < 0) { /* El fichero pedido no existe */

      retorno = _responder_404(connfd, resources_path);
      if (retorno != OK) {
        /* TODO */
      }

    } else { /* El fichero pedido si que existe */

      retorno = _manda_respuesta_con_fichero(connfd, RESPONSE_OK_CODE, RESPONSE_OK_FRASE, ruta_absoluta, fichero_a_mandar_df);
      if (retorno != OK) {
        /* TODO */
      }

    }

    free(ruta_absoluta);
    free(ruta_fichero);

    return OK;
  }

  return OK;
}

int funcionalidad_options(int connfd, char* resources_path) {

  int tamanio_fichero;
  int retorno;
  char *cabecera_respuesta = NULL;
  int cabecera_length;

  /* Reserva memoria para la cabecera de respuesta */
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
  retorno = _cabecera_anadir_codigo_respuesta(cabecera_respuesta, &cabecera_length, RESPONSE_OK_CODE);
  if (retorno != OK) {
    /* TODO */
  }

  /* 1.3 Escribe la frase de respuesta */
  retorno = _cabecera_anadir_frase_respuesta(cabecera_respuesta, &cabecera_length, RESPONSE_OK_FRASE);
  if (retorno != OK) {
    /* TODO */
  }

  /* 2. Escribe los campos de cabecera */

  /* 2.1 Escribe el campo Allow */
  retorno = _cabecera_anadir_allow(cabecera_respuesta, &cabecera_length);
  if (retorno != OK) {
    /* TODO */
  }

  /* 2.2 Escribe el tamaño de fichero (0) */
  retorno = _cabecera_anadir_tamanio_fichero(cabecera_respuesta, &cabecera_length, NULL, &tamanio_fichero);
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


  /*************************************************/

  retorno = _responder_bad_request(connfd, resources_path);
  if (retorno != OK) {
    /* TODO */
  }

  return OK;
}

int ejecutar_script(int connfd, char* resources_path, char *ruta_absoluta) {

  char *ruta_script = NULL;
  char *variables = NULL;
  int   retorno;

  retorno = get_ruta_script_y_variables(ruta_absoluta, &ruta_script, &variables);
  if (retorno != OK) {
    /* TODO */
  }

  /* DEBUG */if (ruta_script == NULL || variables == NULL) {
    printf("Devuelve NULL\n");
  }

  /* DEBUG */printf("Valores pasados al script por GET:\n\tRuta del script: %s\n\tVariables: %s\n", ruta_script, variables);


  /* TODO Ejecutar script pasando las peticiones y mandar respuesta */


  /***************/

  /* DEBUG */
  _responder_bad_request(connfd, resources_path);

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
  }

  extension = punto;

  if (strcmp(extension, EXTENSION_HTML) == 0) {
    return HTML_TYPE;
  } else if (strcmp(extension, EXTENSION_JPG) == 0) {
    return JPG_TYPE;
  } else if (strcmp(extension, EXTENSION_JPEG) == 0) {
    return JPEG_TYPE;
  } else if (strcmp(extension, EXTENSION_ICO) == 0) {
    return ICO_TYPE;
  } else if (strcmp(extension, EXTENSION_GIF) == 0) {
    return GIF_TYPE;
  } else if (strcmp(extension, EXTENSION_TXT) == 0) {
    return TXT_TYPE;
  } else if (strcmp(extension, EXTENSION_MPEG) == 0) {
    return MPEG_TYPE;
  } else if (strcmp(extension, EXTENSION_MP4) == 0) {
    return MP4_TYPE;
  } else if (strcmp(extension, EXTENSION_PDF) == 0) {
    return PDF_TYPE;
  } else if (strcmp(extension, EXTENSION_DOC) == 0) {
    return DOC_TYPE;
  } else if (strcmp(extension, EXTENSION_DOCX) == 0) {
    return DOCX_TYPE;
  } else if (strcmp(extension, EXTENSION_WAV) == 0) {
    return WAV_TYPE;
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

int _cabecera_anadir_allow(char *cabecera_respuesta, int *cabecera_length) {

  char *linea_allow = NULL;

  linea_allow = (char *)calloc(1, MAX_LINEA * sizeof(char));
  if (linea_allow == NULL) {
    /* TODO */
  }

  *cabecera_length += sprintf(linea_allow, "Allow:%s\r\n", ALLOW_LISTA);
  if (linea_allow == NULL) {
    /* TODO */
  }
  strcat(cabecera_respuesta, linea_allow);
  if (cabecera_respuesta == NULL) {
    /* TODO */
  }

  free(linea_allow);
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
    if (tamanio_fichero != NULL) {
      *tamanio_fichero = 0;
    }
  } else {
    if (stat(ruta_fichero, &statbuf) < 0) {
      /* TODO */
      perror("Error con el stat");
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

int _responder_bad_request (int connfd, char* resources_path) {

  char *ruta_absoluta = NULL;
  int retorno;
  int fichero_a_mandar_df;

  /* Actualiza los valores con la página de 404 para los siguientes valores de la cabecera */
  ruta_absoluta = (char *)malloc((strlen(resources_path) + strlen(PAGINA_404) + 1) * sizeof(char));
  if (ruta_absoluta == NULL) {
    /* TODO */
  }

  if (sprintf(ruta_absoluta, "%s%s", resources_path, PAGINA_404) < 0) {
    /* TODO */
  }

  fichero_a_mandar_df = open(ruta_absoluta, O_RDONLY);
  if (fichero_a_mandar_df < 0) {
    /* TODO */
  }

  retorno = _manda_respuesta_con_fichero(connfd, RESPONSE_BAD_REQUEST_CODE, RESPONSE_BAD_REQUEST_FRASE, ruta_absoluta, fichero_a_mandar_df);
  if (retorno != OK) {
    /* TODO */
  }

  free(ruta_absoluta);

  return OK;
}

int _responder_404(int connfd, char *resources_path) {

  char *ruta_absoluta = NULL;
  int retorno;
  int fichero_a_mandar_df;

  /* Actualiza los valores con la página de 404 para los siguientes valores de la cabecera */
  ruta_absoluta = (char *)malloc((strlen(resources_path) + strlen(PAGINA_404) + 1) * sizeof(char));
  if (ruta_absoluta == NULL) {
    /* TODO */
  }

  if (sprintf(ruta_absoluta, "%s%s", resources_path, PAGINA_404) < 0) {
    /* TODO */
  }

  fichero_a_mandar_df = open(ruta_absoluta, O_RDONLY);
  if (fichero_a_mandar_df < 0) {
    /* No se encuentra la página 404 */
    retorno = _responder_404_sin_ficheros(connfd);
    if (retorno != OK) {
      /* TODO */
    }
  }

  retorno = _manda_respuesta_con_fichero(connfd, RESPONSE_NOT_FOUND_CODE, RESPONSE_NOT_FOUND_FRASE, ruta_absoluta, fichero_a_mandar_df);
  if (retorno != OK) {
    /* TODO */
  }

  free(ruta_absoluta);

  return OK;

}

int _responder_404_sin_ficheros(int connfd) {

  int retorno;

  retorno = _manda_respuesta_sin_fichero(connfd, RESPONSE_NOT_FOUND_CODE, RESPONSE_NOT_FOUND_FRASE);
  if (retorno != OK) {
    /* TODO */
  }

  return OK;

}

int _manda_respuesta_con_fichero(int connfd, int codigo, char *frase, char *ruta_absoluta, int fichero_a_mandar_df) {

  char *cabecera_respuesta = NULL;
  int bytes_mandados;
  int cabecera_length = 0;
  int retorno;
  int tamanio_fichero;

  /* Reserva memoria para la cabecera de la respuesta */

  cabecera_respuesta = (char *)calloc(1, MAX_CABECERA * sizeof(char));
  if (cabecera_respuesta == NULL) {
    /* TODO */
  }

  /* Forma la cabecera */

  /* 1.1 Escribe la version */
  retorno = _cabecera_anadir_version_html(cabecera_respuesta, &cabecera_length);
  if (retorno != OK) {
    /* TODO */
  }

  /* 1.2 Escribe el codigo de respuesta */
  retorno = _cabecera_anadir_codigo_respuesta(cabecera_respuesta, &cabecera_length, codigo);
  if (retorno != OK) {
    /* TODO */
  }

  /* 1.3 Escribe la frase de respuesta */
  retorno = _cabecera_anadir_frase_respuesta(cabecera_respuesta, &cabecera_length, frase);
  if (retorno != OK) {
    /* TODO */
  }

  /* 2. Escribe los campos de cabecera */

  /* 2.1 Escribe el tipo de fichero */
  retorno = _cabecera_anadir_tipo_fichero(cabecera_respuesta, &cabecera_length, ruta_absoluta);
  if (retorno != OK) {
    /* TODO */
  }

  /* 2.2 Escribe el tamaño de fichero */
  retorno = _cabecera_anadir_tamanio_fichero(cabecera_respuesta, &cabecera_length, ruta_absoluta, &tamanio_fichero);
  if (retorno != OK) {
    /* TODO */
  }

  /* 2.3. Escribe la fecha y hora actuales */
  retorno = _cabecera_anadir_fecha_y_hora_actual(cabecera_respuesta, &cabecera_length);
  if (retorno != OK) {
    /* TODO */
  }

  /* 2.4. Escribe la hora de ultima modificacion del fichero */
  retorno = _cabecera_anadir_ultima_modificacion(cabecera_respuesta, &cabecera_length, ruta_absoluta);
  if (retorno != OK) {
    /* TODO */
  }

  /* 2.5 Termina la cabecera */
  retorno = _cabecera_terminar(cabecera_respuesta, &cabecera_length);
  if (retorno != OK) {
    /* TODO */
  }


  /* 3. Manda la cabecera */
  if (send(connfd, cabecera_respuesta, cabecera_length, 0) < 0) {
    /* TODO */
    perror("Error en el send");
  }

  /* 4. Manda el archivo */
  retorno = _mandar_fichero_chunks(connfd, fichero_a_mandar_df, tamanio_fichero, TAMANIO_CHUNK, &bytes_mandados);

  if (bytes_mandados < 0) {
    /* TODO */
  } else {
    /* DEBUG */
    printf("Archivo mandado: %s (%d bytes)\n", ruta_absoluta, bytes_mandados);
  }


  if (fichero_a_mandar_df > 0) {
    close(fichero_a_mandar_df);
  }

  free(cabecera_respuesta);

  return OK;

}

int _manda_respuesta_sin_fichero(int connfd, int codigo, char *frase) {

  char *cabecera_respuesta = NULL;
  int cabecera_length = 0;
  int retorno;

  /* Reserva memoria para la cabecera de la respuesta */

  cabecera_respuesta = (char *)calloc(1, MAX_CABECERA * sizeof(char));
  if (cabecera_respuesta == NULL) {
    /* TODO */
  }

  /* Forma la cabecera */

  /* 1.1 Escribe la version */
  retorno = _cabecera_anadir_version_html(cabecera_respuesta, &cabecera_length);
  if (retorno != OK) {
    /* TODO */
  }

  /* 1.2 Escribe el codigo de respuesta */
  retorno = _cabecera_anadir_codigo_respuesta(cabecera_respuesta, &cabecera_length, codigo);
  if (retorno != OK) {
    /* TODO */
  }

  /* 1.3 Escribe la frase de respuesta */
  retorno = _cabecera_anadir_frase_respuesta(cabecera_respuesta, &cabecera_length, frase);
  if (retorno != OK) {
    /* TODO */
  }

  /* 2. Escribe los campos de cabecera */

  /* 2.1 Escribe el tipo de fichero */
  /* TODO: Esto no se necesita */
  /*
  retorno = _cabecera_anadir_tipo_fichero(cabecera_respuesta, &cabecera_length, ruta_absoluta);
  if (retorno != OK) {
    TODO
  }
  */

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

  /* 2.4. Escribe la hora de ultima modificacion del fichero */
  /* TODO: No escribir esto */
  /*
  retorno = _cabecera_anadir_ultima_modificacion(cabecera_respuesta, &cabecera_length, ruta_absoluta);
  if (retorno != OK) {
    TODO
  }
  */

  /* 2.5 Termina la cabecera */
  retorno = _cabecera_terminar(cabecera_respuesta, &cabecera_length);
  if (retorno != OK) {
    /* TODO */
  }


  /* 3. Manda la cabecera */
  if (send(connfd, cabecera_respuesta, cabecera_length, 0) < 0) {
    /* TODO */
    perror("Error en el send");
  }

  /* 4. Manda el archivo */
  /* TODO: No se necesita esto */
  /* retorno = _mandar_fichero_chunks(connfd, fichero_a_mandar_df, tamanio_fichero, TAMANIO_CHUNK, &bytes_mandados);

  if (bytes_mandados < 0) {
    TODO
  } else {
    DEBUG
    printf("Archivo mandado: %s (%d bytes)\n", ruta_absoluta, bytes_mandados);
  }


  if (fichero_a_mandar_df > 0) {
    close(fichero_a_mandar_df);
  } */

  free(cabecera_respuesta);

  return OK;

}




int _fichero_es_script(char *ruta) {

  char *punto         = NULL;
  char *interrogacion = NULL;
  char *ruta_auxiliar = NULL;
  int flag;

  if (ruta == NULL) {
    return FALSE;
  }

  ruta_auxiliar = (char *)malloc((strlen(ruta) + 1) * sizeof(char));
  if (ruta_auxiliar == NULL) {
    /* TODO */
  }
  strcpy(ruta_auxiliar, ruta);
  if (ruta_auxiliar == NULL) {
    /* TODO */
  }

  punto = strrchr(ruta_auxiliar, '.');

  if (punto == NULL || punto == ruta_auxiliar) {
    free(ruta_auxiliar);
    return FALSE;
  }

  interrogacion = strrchr(ruta_auxiliar, '?');

  if (interrogacion == NULL || interrogacion == ruta_auxiliar) {
    free(ruta_auxiliar);
    return FALSE;
  } else {
    interrogacion[0] = '\0';
  }

  if (strcmp(punto, EXTENSION_PYTHON) == 0) {
    flag = TRUE;
  } else if (strcmp(punto, EXTENSION_PHP) == 0) {
    flag = TRUE;
  } else {
    flag = FALSE;
  }

  free(ruta_auxiliar);

  return flag;
}

int get_ruta_script_y_variables(char *ruta_absoluta, char **ruta_script_p, char **variables_p) {

  /* TODO */

  char *interrogacion = NULL;
  char *ruta_auxiliar = NULL;
  char *variables_aux = NULL;

  ruta_auxiliar = (char *)malloc((strlen(ruta_absoluta) + 1) * sizeof(char));
  if (ruta_auxiliar == NULL) {
    /* TODO */
  }
  strcpy(ruta_auxiliar, ruta_absoluta);
  if (ruta_auxiliar == NULL) {
    /* TODO */
  }


  interrogacion = strrchr(ruta_auxiliar, '?');

  if (interrogacion == NULL || interrogacion == ruta_auxiliar) {
    return ERROR;
  } else {
    interrogacion[0] = '\0';

    variables_aux = interrogacion + 1;

    *ruta_script_p = (char *)malloc((strlen(ruta_auxiliar) + 1) * sizeof(char));
    if (*ruta_script_p == NULL) {
      /* TODO */
    }
    strcpy(*ruta_script_p, ruta_auxiliar);
    if (*ruta_script_p == NULL) {
      /* TODO */
    }

    *variables_p = (char *)malloc((strlen(variables_aux) + 1) * sizeof(char));
    if (*variables_p == NULL) {
      /* TODO */
    }
    strcpy(*variables_p, variables_aux);
    if (*variables_p == NULL) {
      /* TODO */
    }


    free(ruta_auxiliar);

    return OK;
  }
}

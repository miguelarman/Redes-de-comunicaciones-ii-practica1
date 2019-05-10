#define _POSIX_C_SOURCE 199309L



/**
 * @brief Librería que procesa cada petición
 *
 * En este fichero, se definen las funciones necesarias
 * para, dada una petición HTTP, generar una respuesta
 * acorde a lo que se pida. Se incluyen algunos errores HTTP
 *
 * @file procesa_peticion.c
 * @author Miguel Arconada Manteca y Mario García Pascual
 * @date 9-5-2019
 */

 #include "../includes/procesa_peticion.h"




/* Valores útiles de dimensiones */
#define MAX_FECHA        512
#define MAX_TIPO_FICHERO 32
#define MAX_CABECERA     10000
#define MAX_LINEA        512
#define MAX_COMANDO      512
#define MAX_PATH         1024
#define MAX_RESULTADO    4096

#define TAMANIO_CHUNK    1024

/* Paths de páginas de errores */
#define INDEX_BASICO               "/www/index.html"
#define PAGINA_BAD_REQUEST         "/www/400.html"
#define PAGINA_NOT_FOUND           "/www/404.html"
#define PAGINA_SERVER_ERROR        "/www/500.html"
#define PAGINA_SERVICE_UNAVAILABLE "/www/503.html"

#define HTTP_RESPONSE_VERSION "HTTP/1.1"

#define ALLOW_LISTA "OPTIONS, GET, POST"

#define RESPONSE_OK_CODE                  200
#define RESPONSE_BAD_REQUEST_CODE         400
#define RESPONSE_NOT_FOUND_CODE           404
#define RESPONSE_SERVER_ERROR_CODE        500
#define RESPONSE_SERVICE_UNAVAILABLE_CODE 503

#define RESPONSE_OK_FRASE                    "OK"
#define RESPONSE_BAD_REQUEST_FRASE           "Bad Request"
#define RESPONSE_NOT_FOUND_FRASE             "Not Found"
#define RESPONSE_SERVER_ERROR_FRASE          "Internal Server Error"
#define RESPONSE_SERVICE_UNAVAILABLE_FRASE   "Service Unavailable"

/* Ficheros auxiliares */
#define FICHERO_AUX_PATH     "/fichero_aux.txt"
#define RUTA_FICHERO_RETORNO "/resultados.html"

/* Extensiones de ficheros */
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

/* Valores del campo content-type */
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

#define PYTHON 10
#define PHP 11
#define OTRO_TIPO 12

/* Códigos de retorno de funciones */
#define ERROR                     -1
#define OK                         0
#define TRUE                       1
#define FALSE                      0
#define SCRIPT_NO_EXISTE           20
#define SCRIPT_NO_SOPORTADO        21
#define RESOURCES_PATH_NULL_ERROR  22
#define DESCRIPTOR_CONN_INCORRECTO 23
#define SERVER_ERROR               24
#define SEND_ERROR                 25
#define PIPE_ERROR                 26
#define PETICION_INVALIDA          27



/**
 * @brief Función auxiliar que analiza el tipo de fichero por su extensión
 *
 * Esta función analiza el tipo del fichero
 *
 * @ingroup Procesa_peticion
 * @param ruta Ruta del fichero
 * @return Tipo del archivo
 */
char * _tipo_archivo(char *ruta);

/**
 * @brief Función auxiliar que analiza el tipo de script por su extensión
 *
 * Esta función analiza el tipo del script
 *
 * @param ruta Ruta del script
 * @return Tipo del archivo
 */
int _tipo_script(char *ruta);



/**
 * @brief Función que ejecuta un script
 *
 * Esta función ejecuta un script en la terminal y devuelve la salida
 *
 * @ingroup Procesa_peticion
 * @param ruta_script Ruta del script
 * @param resources_path Ruta raíz de la carpeta resources/
 * @param variables_get Variables explicitadas en la url de la petición
 * @param bodylen Tamaño del body de la petición
 * @param body Cuerpo de la petición
 * @param resultado Resultado de la ejecución del script (salida)
 * @return Código de ejecución
 */
int _invoca_script(char* ruta_script, char* resources_path, char* variables_get, int bodylen, char *body, char* resultado);

/**
 * @brief Función que comprueba si un fichero es un script
 *
 * Esta función comprueba la extensión de un fichero
 * para ver si es un script
 *
 * @ingroup Procesa_peticion
 * @param ruta Ruta del script
 * @return Valor booleano
 */
int _fichero_es_script(char *ruta);

/**
 * @brief Función que separa fichero y argumentos
 *
 * Esta función, de una url con parámetros, separa la ruta del
 * fichero y los argumentos a pasar al mismo
 *
 * @ingroup Procesa_peticion
 * @param ruta_absoluta Ruta del script con argumentos
 * @param ruta_script_p Variable de salida de ruta del script
 * @param variables_p Variable de variables del script
 * @return Valor booleano
 */
int _get_ruta_script_y_variables(char *ruta_absoluta, char **ruta_script_p, char **variables_p);

/****************************************************/
/* Funciones que escriben cada campo de la cabecera */
/****************************************************/

/**
 * @brief Función que escribe la hora actual
 *
 * Esta función calcula la hora actual, y lo añade a
 * la cabecera de la respuesta
 *
 * @ingroup Procesa_peticion
 * @param cabecera_respuesta Cabecera de la respuesta
 * @param cabecera_length Cantidad de bytes leídos (salida)
 * @return Código de ejecución
 */
int _cabecera_anadir_fecha_y_hora_actual(char *cabecera_respuesta, int *cabecera_length);

/**
 * @brief Función que escribe el tipo de fichero
 *
 * Esta función calcula el tipo de fichero, y lo añade a
 * la cabecera de la respuesta
 *
 * @ingroup Procesa_peticion
 * @param cabecera_respuesta Cabecera de la respuesta
 * @param cabecera_length Cantidad de bytes leídos (salida)
 * @param ruta_fichero Ruta del fichero
 * @return Código de ejecución
 */
int _cabecera_anadir_tipo_fichero(char *cabecera_respuesta, int *cabecera_length, char *ruta_fichero);

/**
 * @brief Función que escribe la última modificación de un fichero
 *
 * Esta función calcula la última modificación de un
 * fichero, y lo añade a la cabecera de la respuesta
 *
 * @ingroup Procesa_peticion
 * @param cabecera_respuesta Cabecera de la respuesta
 * @param cabecera_length Cantidad de bytes leídos (salida)
 * @param ruta_fichero Ruta del fichero
 * @return Código de ejecución
 */
int _cabecera_anadir_ultima_modificacion(char *cabecera_respuesta, int *cabecera_length, char *ruta_fichero);

/**
 * @brief Función que escribe la lista de comandos permitidos
 *
 * Esta función forma la línea de Allow, y
 * lo añade a la cabecera de la respuesta
 *
 * @ingroup Procesa_peticion
 * @param cabecera_respuesta Cabecera de la respuesta
 * @param cabecera_length Cantidad de bytes leídos (salida)
 * @return Código de ejecución
 */
int _cabecera_anadir_allow(char *cabecera_respuesta, int *cabecera_length);

/**
 * @brief Función que escribe el tamaño de un fichero
 *
 * Esta función calcula el tamaño de un fichero, y
 * lo añade a la cabecera de la respuesta
 *
 * @ingroup Procesa_peticion
 * @param cabecera_respuesta Cabecera de la respuesta
 * @param cabecera_length Cantidad de bytes leídos (salida)
 * @param ruta_fichero Ruta del fichero
 * @param tamanio_fichero Variable donde se guarda el tamaño del fichero
 * @return Código de ejecución
 */
int _cabecera_anadir_tamanio_fichero(char *cabecera_respuesta, int *cabecera_length, char *ruta_fichero, int *tamanio_fichero);

/**
 * @brief Función que termina una cabecera
 *
 * Esta función añade a la cabecera de una respuesta
 * la separación entre la cabecera y el cuerpo
 *
 * @ingroup Procesa_peticion
 * @param cabecera_respuesta Cabecera de la respuesta
 * @param cabecera_length Cantidad de bytes leídos (salida)
 * @return Código de ejecución
 */
int _cabecera_terminar(char *cabecera_respuesta, int *cabecera_length);

/**
 * @brief Función que escribe la versión HTML
 *
 * Esta función escribe en la cabecera de
 * respuesta la versión HTTP de la respuesta
 *
 * @ingroup Procesa_peticion
 * @param cabecera_respuesta Cabecera de la respuesta
 * @param cabecera_length Cantidad de bytes leídos (salida)
 * @return Código de ejecución
 */
int _cabecera_anadir_version_html(char *cabecera_respuesta, int *cabecera_length);

/**
 * @brief Función que escribe el código de respuesta
 *
 * Esta función escribe en la cabecera de
 * respuesta el código de respuesta
 *
 * @ingroup Procesa_peticion
 * @param cabecera_respuesta Cabecera de la respuesta
 * @param cabecera_length Cantidad de bytes leídos (salida)
 * @param codigo Código de respuesta
 * @return Código de ejecución
 */
int _cabecera_anadir_codigo_respuesta(char *cabecera_respuesta, int *cabecera_length, int codigo);

/**
 * @brief Función que escribe la frase de respuesta
 *
 * Esta función escribe en la cabecera de
 * respuesta la frase de respuesta
 *
 * @ingroup Procesa_peticion
 * @param cabecera_respuesta Cabecera de la respuesta
 * @param cabecera_length Cantidad de bytes leídos (salida)
 * @param frase Frase de respuesta
 * @return Código de ejecución
 */
int _cabecera_anadir_frase_respuesta(char *cabecera_respuesta, int *cabecera_length, char *frase);

/* Funciones de red */

/**
 * @brief Función que envía un fichero en chunks
 *
 * Esta función envía a un socket un fichero por
 * chunks de un tamaño fijo
 *
 * @ingroup Procesa_peticion
 * @param connfd Descriptor del socket
 * @param fichero_a_mandar_df Descriptor del fichero a mandar
 * @param tamanio_fichero Tamaño total del fichero
 * @param tamanio_chunk Tamaño de cada chunk a enviar
 * @param bytes_mandados Salida de bytes mandados
 * @return Código de ejecución
 */
int _mandar_fichero_chunks(int connfd, int fichero_a_mandar_df, int tamanio_fichero, int tamanio_chunk, int *bytes_mandados);



/* Funciones de retorno de error */

/**
 * @brief Función que responde un Bad Request
 *
 * Esta función responde con una respuesta
 * Bad Request a la petición anterior
 *
 * @ingroup Procesa_peticion
 * @param connfd Descriptor del socket
 * @param resources_path Ruta de los recursos
 * @return Código de ejecución
 */
int _responder_bad_request (int connfd, char* resources_path);

/**
 * @brief Función que responde un Bad Request sin enviar fichero
 *
 * Esta función responde con una respuesta Bad Request
 * a la petición anterior sin enviar ningún fichero adicional
 *
 * @ingroup Procesa_peticion
 * @param connfd Descriptor del socket
 * @return Código de ejecución
 */
int _responder_bad_request_sin_ficheros (int connfd);

/**
 * @brief Función que responde un Service Unavailable
 *
 * Esta función responde con una respuesta
 * Service Unavailable a la petición anterior
 *
 * @ingroup Procesa_peticion
 * @param connfd Descriptor del socket
 * @param resources_path Ruta de los recursos
 * @return Código de ejecución
 */
int _responder_service_unavailable (int connfd, char* resources_path);

/**
 * @brief Función que responde un Service Unavailable sin enviar fichero
 *
 * Esta función responde con una respuesta Service Unavailable
 * a la petición anterior sin enviar ningún fichero adicional
 *
 * @ingroup Procesa_peticion
 * @param connfd Descriptor del socket
 * @return Código de ejecución
 */
int _responder_service_unavailable_sin_ficheros (int connfd);

/**
 * @brief Función que responde un Server Error
 *
 * Esta función responde con una respuesta
 * Server Error a la petición anterior
 *
 * @ingroup Procesa_peticion
 * @param connfd Descriptor del socket
 * @param resources_path Ruta de los recursos
 * @return Código de ejecución
 */
int _responder_server_error (int connfd, char* resources_path);

/**
 * @brief Función que responde un Server Error sin enviar fichero
 *
 * Esta función responde con una respuesta Server Error
 * a la petición anterior sin enviar ningún fichero adicional
 *
 * @ingroup Procesa_peticion
 * @param connfd Descriptor del socket
 * @return Código de ejecución
 */
int _responder_server_error_sin_ficheros (int connfd);

/**
 * @brief Función que responde un Not Found
 *
 * Esta función responde con una respuesta
 * Not Found a la petición anterior
 *
 * @ingroup Procesa_peticion
 * @param connfd Descriptor del socket
 * @param resources_path Ruta de los recursos
 * @return Código de ejecución
 */
int _responder_not_found (int connfd, char *resources_path);

/**
 * @brief Función que responde un Not Found sin enviar fichero
 *
 * Esta función responde con una respuesta Not Found
 * a la petición anterior sin enviar ningún fichero adicional
 *
 * @ingroup Procesa_peticion
 * @param connfd Descriptor del socket
 * @return Código de ejecución
 */
int _responder_not_found_sin_ficheros (int connfd);





/********************************************/
/********************************************/
/********************************************/
/*       IMPLEMENTACIÓN DE FUNCIONES        */
/********************************************/
/********************************************/
/********************************************/


int procesa_peticion (int connfd, char *resources_path, Parsear campos_parseados) {

  char *verbo_peticion      = NULL;
  char *ruta_fichero        = NULL;
  char *method              = NULL;
  char *path                = NULL;
  char *body                = NULL;
  int   bodylen;
  int   retorno;
  size_t  method_len;
  size_t  path_len;



  /* Guardo los valores guardados por parsear_peticion en la estructura */
  method        = campos_parseados.method;
  path          = campos_parseados.path;
  body          = campos_parseados.body;
  bodylen       = campos_parseados.bodylen;
  method_len    = campos_parseados.method_len;
  path_len      = campos_parseados.path_len;


  if (method == NULL) {
    /* El navegador a veces genera peticiones incorrectas */
    syslog(LOG_INFO, "Petición incorrecta (metodo es null)");
    _responder_bad_request(connfd, resources_path);
    return PETICION_INVALIDA;
  }

  syslog(LOG_INFO, "Recibida petición %.*s. Path: %.*s", (int)method_len, method, (int)path_len, path);

  /* Comprobaciones de argumentos */
  if (resources_path == NULL) {
    syslog(LOG_INFO, "Resources_path incorrecta (no puede ser NULL)");
    return RESOURCES_PATH_NULL_ERROR;
  }

  if (connfd < 0) {
    syslog(LOG_INFO, "Descriptor del socket incorrecto");
    return DESCRIPTOR_CONN_INCORRECTO;
  }


  if ((verbo_peticion = (char *)malloc(((int)method_len + 1) * sizeof(char))) == NULL) {
    syslog(LOG_ERR, "Error en malloc");
    _responder_server_error(connfd, resources_path);
    return SERVER_ERROR;
  }
  if (sprintf(verbo_peticion, "%.*s", (int)method_len, method) < 0) {
    syslog(LOG_ERR, "Error en sprintf");
    _responder_server_error(connfd, resources_path);
    if (verbo_peticion) free(verbo_peticion);
    return SERVER_ERROR;
  }

  /* Guarda la ruta del fichero */
  ruta_fichero = (char *)malloc(((int)path_len + 1) * sizeof(char));
  if (ruta_fichero == NULL) {
    syslog(LOG_ERR, "Error en malloc");
    _responder_server_error(connfd, resources_path);
    if (verbo_peticion) free(verbo_peticion);
    return SERVER_ERROR;
  }
  if (sprintf(ruta_fichero, "%.*s", (int)path_len, path) < 0) {
    syslog(LOG_ERR, "Error en sprintf");
    _responder_server_error(connfd, resources_path);
    if (verbo_peticion) free(verbo_peticion);
    if (ruta_fichero) free(ruta_fichero);
    return SERVER_ERROR;
  }

  if (strcmp(verbo_peticion, "GET") == 0) { /* Petición GET */


    retorno = funcionalidad_get(ruta_fichero, resources_path, connfd, bodylen, body);
    if (retorno != OK) {
      syslog(LOG_ERR, "La funcionalidad GET no ha funcionado correctamente (%d)", retorno);
      _responder_server_error(connfd, resources_path);
      if (verbo_peticion) free(verbo_peticion);
      if (ruta_fichero) free(ruta_fichero);
      return SERVER_ERROR;
    }

  } else if (strcmp(verbo_peticion, "OPTIONS") == 0) { /* Petición OPTIONS */

    retorno = funcionalidad_options(connfd, resources_path);
    if (retorno != OK) {
      syslog(LOG_ERR, "La funcionalidad GET no ha funcionado correctamente (%d)", retorno);
      _responder_server_error(connfd, resources_path);
      if (verbo_peticion) free(verbo_peticion);
      if (ruta_fichero) free(ruta_fichero);
      return SERVER_ERROR;
    }

  } else if (strcmp(verbo_peticion, "POST") == 0) { /* Petición POST */

    retorno = funcionalidad_post(ruta_fichero, resources_path, connfd, bodylen, body);
    if (retorno != OK) {
      syslog(LOG_ERR, "La funcionalidad GET no ha funcionado correctamente (%d)", retorno);
      _responder_server_error(connfd, resources_path);
      if (verbo_peticion) free(verbo_peticion);
      if (ruta_fichero) free(ruta_fichero);
      return SERVER_ERROR;
    }


  } else {
    /* Verbo no soportado */
    retorno = _responder_bad_request(connfd, resources_path);
    if (retorno != OK) {
      syslog(LOG_ERR, "La función bad request no ha funcionado correctamente (%d)", retorno);
      _responder_server_error(connfd, resources_path);
      if (verbo_peticion) free(verbo_peticion);
      if (ruta_fichero) free(ruta_fichero);
      return SERVER_ERROR;
    }

    /*
    retorno = _responder_service_unavailable(connfd, resources_path);
    if (retorno != OK) {
      syslog(LOG_ERR, "La función bad request no ha funcionado correctamente (%d)", retorno);
      _responder_server_error(connfd, resources_path);
      if (verbo_peticion) free(verbo_peticion);
      if (ruta_fichero) free(ruta_fichero);
      return SERVER_ERROR;
    }
    */
  }

  free(ruta_fichero);
  free(verbo_peticion);

  return OK;
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
  campos_a_parsear->bodylen = campos_a_parsear->buflen - campos_a_parsear->pret;
  campos_a_parsear->body = campos_a_parsear->buf + campos_a_parsear->pret;

  return OK;
}

int funcionalidad_get(char *ruta_fichero, char *resources_path, int connfd, int bodylen, char *body) {

  int   fichero_a_mandar_df = -1;
  int   retorno             =  ERROR;
  char *ruta_absoluta       = NULL;

  /* Guarda la ruta relativa del fichero pedido */
  if (strcmp(ruta_fichero, "/") == 0) {
    /* Responder html básico */
    ruta_absoluta = (char *)malloc((strlen(INDEX_BASICO) + strlen(resources_path) + 1) * sizeof(char));
    if (ruta_absoluta == NULL) {
      syslog(LOG_ERR, "Error en malloc");
      return SERVER_ERROR;
    }

    if (sprintf(ruta_absoluta, "%s%s", resources_path, INDEX_BASICO) < 0) {
      syslog(LOG_ERR, "Error en sprintf");
      if (ruta_absoluta) free(ruta_absoluta);
      return SERVER_ERROR;
    }

  } else {
    ruta_absoluta = (char *)malloc((strlen(ruta_fichero) + strlen(resources_path) + 1) * sizeof(char));
    if (ruta_absoluta == NULL) {
      syslog(LOG_ERR, "Error en malloc");
      return SERVER_ERROR;
    }

    if (sprintf(ruta_absoluta, "%s%s", resources_path, ruta_fichero) < 0) {
      syslog(LOG_ERR, "Error en sprintf");
      if (ruta_absoluta) free(ruta_absoluta);
      return SERVER_ERROR;
    }
  }



  /* Peticion GET a un script */
  if (_fichero_es_script(ruta_absoluta) == TRUE) {

    ejecutar_script(connfd, resources_path, ruta_absoluta, bodylen, body);
    /* No comprobamos retorno pues esta función siempre va a enviar algo */

  } else { /* Se ha solicitado un fichero */

    /* Abre el fichero pedido */
    fichero_a_mandar_df = open(ruta_absoluta, O_RDONLY);

    if (fichero_a_mandar_df < 0) { /* El fichero pedido no existe */

      _responder_not_found(connfd, resources_path);

    } else { /* El fichero pedido si que existe */

      retorno = _manda_respuesta_con_fichero(connfd, RESPONSE_OK_CODE, RESPONSE_OK_FRASE, ruta_absoluta, fichero_a_mandar_df);
      if (retorno != OK) {
        syslog(LOG_ERR, "Error la funcion _manda_respuesta_con_fichero (%d)", retorno);
        if (ruta_absoluta) free(ruta_absoluta);
        return SERVER_ERROR;
      }

    }

    free(ruta_absoluta);

    return OK;
  }

  return OK;
}

int funcionalidad_post(char *ruta_fichero, char *resources_path, int connfd, int bodylen, char *body) {
  char *ruta_absoluta = NULL;


  ruta_absoluta = (char *)malloc((strlen(ruta_fichero) + strlen(resources_path) + 1) * sizeof(char));
  if (ruta_absoluta == NULL) {
    syslog(LOG_ERR, "Error en malloc");
    return SERVER_ERROR;
  }

  if (sprintf(ruta_absoluta, "%s%s", resources_path, ruta_fichero) < 0) {
    syslog(LOG_ERR, "Error en sprintf");
    if (ruta_absoluta) free(ruta_absoluta);
    return SERVER_ERROR;
  }

  ejecutar_script(connfd, resources_path, ruta_absoluta, bodylen, body);
  /* No comprobamos el retorno pues esta función siempre envía algo */

  free(ruta_absoluta);

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
    syslog(LOG_ERR, "Error en calloc");
    return SERVER_ERROR;
  }

  /* 1.1 Escribe la version */
  retorno = _cabecera_anadir_version_html(cabecera_respuesta, &cabecera_length);
  if (retorno != OK) {
    syslog(LOG_ERR, "Error en _cabecera_anadir_version_html (%d)", retorno);
    if (cabecera_respuesta) free(cabecera_respuesta);
    return SERVER_ERROR;
  }

  /* 1.2 Escribe el codigo de respuesta */
  retorno = _cabecera_anadir_codigo_respuesta(cabecera_respuesta, &cabecera_length, RESPONSE_OK_CODE);
  if (retorno != OK) {
    syslog(LOG_ERR, "Error en _cabecera_anadir_codigo_respuesta (%d)", retorno);
    if (cabecera_respuesta) free(cabecera_respuesta);
    return SERVER_ERROR;
  }

  /* 1.3 Escribe la frase de respuesta */
  retorno = _cabecera_anadir_frase_respuesta(cabecera_respuesta, &cabecera_length, RESPONSE_OK_FRASE);
  if (retorno != OK) {
    syslog(LOG_ERR, "Error en _cabecera_anadir_frase_respuesta (%d)", retorno);
    if (cabecera_respuesta) free(cabecera_respuesta);
    return SERVER_ERROR;
  }

  /* 2. Escribe los campos de cabecera */

  /* 2.1 Escribe el campo Allow */
  retorno = _cabecera_anadir_allow(cabecera_respuesta, &cabecera_length);
  if (retorno != OK) {
    syslog(LOG_ERR, "Error en _cabecera_anadir_allow (%d)", retorno);
    if (cabecera_respuesta) free(cabecera_respuesta);
    return SERVER_ERROR;
  }

  /* 2.2 Escribe el tamaño de fichero (0) */
  retorno = _cabecera_anadir_tamanio_fichero(cabecera_respuesta, &cabecera_length, NULL, &tamanio_fichero);
  if (retorno != OK) {
    syslog(LOG_ERR, "Error en _cabecera_anadir_tamanio_fichero (%d)", retorno);
    if (cabecera_respuesta) free(cabecera_respuesta);
    return SERVER_ERROR;
  }

  /* 2.3. Escribe la fecha y hora actuales */
  retorno = _cabecera_anadir_fecha_y_hora_actual(cabecera_respuesta, &cabecera_length);
  if (retorno != OK) {
    syslog(LOG_ERR, "Error en _cabecera_anadir_fecha_y_hora_actual (%d)", retorno);
    if (cabecera_respuesta) free(cabecera_respuesta);
    return SERVER_ERROR;
  }

  /* 2.5 Termina la cabecera */
  retorno = _cabecera_terminar(cabecera_respuesta, &cabecera_length);
  if (retorno != OK) {
    syslog(LOG_ERR, "Error en _cabecera_terminar (%d)", retorno);
    if (cabecera_respuesta) free(cabecera_respuesta);
    return SERVER_ERROR;
  }

  /* 3. Manda la cabecera */
  if (send(connfd, cabecera_respuesta, cabecera_length, 0) < 0) {
    syslog(LOG_ERR, "Error al mandar la cabecera");
    if (cabecera_respuesta) free(cabecera_respuesta);
    return SEND_ERROR;
  }

  free(cabecera_respuesta);

  return OK;
}

int ejecutar_script(int connfd, char* resources_path, char *ruta_absoluta, int bodylen, char *body) {

  FILE *pf = NULL;
  char *ruta_script = NULL;
  char *variables_get = NULL;
  char  ruta_fichero_retorno[MAX_PATH];
  char  resultado_script[MAX_RESULTADO];
  int   retorno;
  int   fichero_a_mandar_df;
  int   ret;


  retorno = _get_ruta_script_y_variables(ruta_absoluta, &ruta_script, &variables_get);
  if (retorno != OK) {
    syslog(LOG_ERR, "Error en la función _get_ruta_script_y_variables (%d)", retorno);
    _responder_server_error(connfd, resources_path);
    return SERVER_ERROR;
  }


  /* Ejecutar script pasando las peticiones y mandar respuesta */
  ret = _invoca_script(ruta_script, resources_path, variables_get, bodylen, body, resultado_script);
  if (ret != OK) {
    if (ret == SCRIPT_NO_EXISTE) {
      syslog(LOG_ERR, "El script solicitado no existe");
      _responder_not_found(connfd, resources_path);
      return OK;
    } else {
      syslog(LOG_ERR, "Error en la función _invoca_script (%d)", retorno);
      _responder_server_error(connfd, resources_path);
      return SERVER_ERROR;
    }
  }


  /* Guardamos la respuesta en un fichero para mandarlo */
  strcpy(ruta_fichero_retorno, resources_path);
  strcat(ruta_fichero_retorno, RUTA_FICHERO_RETORNO);

  pf = fopen(ruta_fichero_retorno, "w");
  if (pf == NULL) {
    syslog(LOG_ERR, "Error al crear el fichero de retorno del script");
    _responder_server_error(connfd, resources_path);
    return SERVER_ERROR;
  }
  fprintf(pf, "%s", resultado_script);
  fclose(pf);

  /* Responde con el fichero creado */
  fichero_a_mandar_df = open(ruta_fichero_retorno, O_RDONLY);
  if (fichero_a_mandar_df < 0) {
    syslog(LOG_ERR, "Error al abrir el fichero de retorno del script");
    _responder_server_error(connfd, resources_path);
    return SERVER_ERROR;
  }
  retorno = _manda_respuesta_con_fichero(connfd, RESPONSE_OK_CODE, RESPONSE_OK_FRASE, ruta_fichero_retorno, fichero_a_mandar_df);
  close(fichero_a_mandar_df);

  /* Elimina el fichero creado */
  if (remove(ruta_fichero_retorno) < 0) {
    syslog(LOG_INFO, "No se ha podido borrar el fichero %s", ruta_fichero_retorno);
    _responder_server_error(connfd, resources_path);
    return SERVER_ERROR;
  }

  free(ruta_script);
  free(variables_get);

  return OK;
}

int _invoca_script(char* ruta_script, char* resources_path, char* variables_get, int bodylen, char *body, char* resultado) {
    char* extension;
    char comando[MAX_COMANDO];
    FILE* pipe;
    FILE* fp;
    char fichero_args_aux[MAX_PATH];
    int b_leidos;
    int fp_aux;

    /* Lo primero comprobamos si el script pedido existe */
    fp_aux = open(ruta_script, O_RDONLY);
    if (fp_aux < 0) {
      syslog(LOG_INFO, "Se ha solicitado un script no existente: %s", ruta_script);
      return SCRIPT_NO_EXISTE;
    }
    close(fp_aux);

    /* Vemos el tipo de script nos es pedido */
    extension = strrchr(ruta_script, '.');
    if (extension == NULL) {
      syslog(LOG_ERR, "Error al leer la extensión del script %s", ruta_script);
      return SERVER_ERROR;
    }

    /* Solo soportamos scripts .php y .py*/
    if (_tipo_script(ruta_script) == PYTHON) {
      sprintf(comando, "python3");
    } else if (_tipo_script(ruta_script) == PHP){
      sprintf(comando, "php");
    } else {
      printf("Script no soportado: %s\n", extension);
      syslog(LOG_INFO, "Se ha solicitado un script no soportado: %s", ruta_script);
      return SCRIPT_NO_SOPORTADO;
    }


    /* Añadimos al comando la path del script a ejecutar */
    strcat(comando, " ");
    strcat(comando, ruta_script);

    /* Abrimos un fichero auxiliar en el que escribir los argumentos */
    strcpy(fichero_args_aux, resources_path);
    strcat(fichero_args_aux, FICHERO_AUX_PATH);
    fp = fopen(fichero_args_aux, "w");
    if (fp == NULL) {
      syslog(LOG_ERR, "No se ha podido crear el fichero %s", fichero_args_aux);
      return SERVER_ERROR;
    }


    /* Si hay argumentos por GET los añade */
    if (variables_get != NULL) {
      syslog(LOG_DEBUG, "Se han introducidos por la url: %s", variables_get);

      /* Escribimos en el fichero los argumentos recibidos por url (GET) */
      fprintf(fp, "%s", variables_get);

      /* Si hay, añadimos los argumentos por POST*/
      if (bodylen > 0) {
        syslog(LOG_DEBUG, "Se han introducidos por el body: %.*s", bodylen, body);

        /* Argumentos pasados por el body (POST) */
        fprintf(fp, "&%.*s", bodylen, body);
      }

      /* Cerramos el fichero y decimos en el comando que se lea de ahí*/
      strcat(comando, " < ");
      strcat(comando,  fichero_args_aux);

    } else {
      if (bodylen > 0) {
        syslog(LOG_DEBUG, "Se han introducidos por el body: %.*s", bodylen, body);

        /* Argumentos pasados por el body (POST) */
        fprintf(fp, "%.*s", bodylen, body);


        strcat(comando, " < ");
        strcat(comando,  fichero_args_aux);
      } else {
        /* No hay argumentos */
        syslog(LOG_DEBUG, "No se han introducido argumentos para el script");

        strcat(comando, " < /dev/null");
      }
    }

    /* Cerramos el fichero auxiliar para que se termine de crear */
    fclose(fp);

    /* Ejecuta el script mediante una pipe */
    syslog(LOG_INFO, "Ejecutando: %s", comando);

    pipe = popen(comando, "r");
    if (pipe == NULL) {
      syslog(LOG_ERR, "Error en la pipe con el comando %s", comando);
      return PIPE_ERROR;
    }

    /* Leemos el resultado de la pipe */
    b_leidos = fread(resultado, sizeof(char), MAX_RESULTADO, pipe);
    pclose(pipe);

    /* Terminamos la cadena leída con un \0 */
    resultado[b_leidos] = 0;

    /* Borramos el fichero auxiliar creado */
    remove(fichero_args_aux);
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

int _tipo_script(char *ruta) {

  char *punto = NULL;
  char *extension = NULL;

  if (ruta == NULL) {
    return OTRO_TIPO;
  }

  punto = strrchr(ruta, '.');

  if (punto == NULL || punto == ruta) {
    return OTRO_TIPO;
  }

  extension = punto;


  if (strcmp(extension, EXTENSION_PYTHON) == 0) {
    return PYTHON;
  } else if (strcmp(extension, EXTENSION_PHP) == 0) {
    return PHP;
  } else {
    return OTRO_TIPO;
  }
}

int _cabecera_anadir_fecha_y_hora_actual(char *cabecera_respuesta, int *cabecera_length) {
  time_t now = time(0);
  struct tm tm = *gmtime(&now);
  char *linea_fecha = NULL;

  linea_fecha = (char *)calloc(1, MAX_LINEA * sizeof(char));
  if (linea_fecha == NULL) {
    syslog(LOG_ERR, "Error en el malloc");
    return SERVER_ERROR;
  }

  *cabecera_length += strftime(linea_fecha, MAX_FECHA, "Date: %a, %d %b %Y %H:%M:%S %Z\r\n", &tm);
  if (linea_fecha == NULL) {
    syslog(LOG_ERR, "Error en el strftime");
    if (linea_fecha) free(linea_fecha);
    return SERVER_ERROR;
  }
  strcat(cabecera_respuesta, linea_fecha);
  if (cabecera_respuesta == NULL) {
    syslog(LOG_ERR, "Error en el strcat");
    if (linea_fecha) free(linea_fecha);
    return SERVER_ERROR;
  }

  free(linea_fecha);
  return OK;
}

int _cabecera_anadir_tipo_fichero(char *cabecera_respuesta, int *cabecera_length, char *ruta_fichero) {

  char *tipo_fichero = NULL;
  char *linea_tipo = NULL;

  linea_tipo = (char *)calloc(1, MAX_LINEA * sizeof(char));
  if (linea_tipo == NULL) {
    syslog(LOG_ERR, "Error en el malloc");
    return SERVER_ERROR;
  }

  tipo_fichero = (char *)calloc(1, MAX_TIPO_FICHERO * sizeof(char));
  if (tipo_fichero == NULL) {
    syslog(LOG_ERR, "Error en el calloc");
    if (linea_tipo) free(linea_tipo);
    return SERVER_ERROR;
  }

  strcpy(tipo_fichero, _tipo_archivo(ruta_fichero));
  if (tipo_fichero == NULL) {
    syslog(LOG_ERR, "Error en el strcpy");
    if (linea_tipo) free(linea_tipo);
    if (tipo_fichero) free(tipo_fichero);
    return SERVER_ERROR;
  }

  *cabecera_length += sprintf(linea_tipo, "Content-type:%s\r\n", tipo_fichero);
  if (linea_tipo == NULL) {
    syslog(LOG_ERR, "Error en el sprintf");
    if (linea_tipo) free(linea_tipo);
    if (tipo_fichero) free(tipo_fichero);
    return SERVER_ERROR;
  }
  strcat(cabecera_respuesta, linea_tipo);
  if (cabecera_respuesta == NULL) {
    syslog(LOG_ERR, "Error en el strcat");
    if (linea_tipo) free(linea_tipo);
    if (tipo_fichero) free(tipo_fichero);
    return SERVER_ERROR;
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
    syslog(LOG_ERR, "Error en el malloc");
    return SERVER_ERROR;
  }

  if (stat(ruta_fichero, &statbuf) < 0) {
    syslog(LOG_ERR, "Error en el stat");
    if (linea_modificado) free(linea_modificado);
    return SERVER_ERROR;
  }

  last_modified.tv_sec = statbuf.st_mtime;
  modificado = gmtime(&(last_modified.tv_sec));

  *cabecera_length += strftime(linea_modificado, MAX_FECHA, "Last-modified: %a, %d %b %Y %H:%M:%S %Z\r\n", modificado);
  if (linea_modificado == NULL) {
    syslog(LOG_ERR, "Error en el strftime");
    if (linea_modificado) free(linea_modificado);
    return SERVER_ERROR;
  }
  strcat(cabecera_respuesta, linea_modificado);
  if (cabecera_respuesta == NULL) {
    syslog(LOG_ERR, "Error en el strcat");
    if (linea_modificado) free(linea_modificado);
    return SERVER_ERROR;
  }

  free(linea_modificado);
  return OK;
}

int _cabecera_anadir_allow(char *cabecera_respuesta, int *cabecera_length) {

  char *linea_allow = NULL;

  linea_allow = (char *)calloc(1, MAX_LINEA * sizeof(char));
  if (linea_allow == NULL) {
    syslog(LOG_ERR, "Error en el calloc");
    if (linea_allow) free(linea_allow);
    return SERVER_ERROR;
  }

  *cabecera_length += sprintf(linea_allow, "Allow:%s\r\n", ALLOW_LISTA);
  if (linea_allow == NULL) {
    syslog(LOG_ERR, "Error en el sprintf");
    if (linea_allow) free(linea_allow);
    return SERVER_ERROR;
  }
  strcat(cabecera_respuesta, linea_allow);
  if (cabecera_respuesta == NULL) {
    syslog(LOG_ERR, "Error en el strcat");
    if (linea_allow) free(linea_allow);
    return SERVER_ERROR;
  }

  free(linea_allow);
  return OK;

}

int _cabecera_anadir_tamanio_fichero(char *cabecera_respuesta, int *cabecera_length, char *ruta_fichero, int *tamanio_fichero) {

  struct stat statbuf;
  char *linea_tamanio = NULL;

  linea_tamanio = (char *)calloc(1, MAX_LINEA * sizeof(char));
  if (linea_tamanio == NULL) {
    syslog(LOG_ERR, "Error en el calloc");
    return SERVER_ERROR;
  }

  if (ruta_fichero == NULL) {
      *tamanio_fichero = 0;
  } else {
    if (stat(ruta_fichero, &statbuf) < 0) {
      syslog(LOG_ERR, "Error en el stat");
      if (linea_tamanio) free(linea_tamanio);
      return SERVER_ERROR;
    }
    *tamanio_fichero = statbuf.st_size;
  }

  *cabecera_length += sprintf(linea_tamanio, "Content-length:%d\r\n", *tamanio_fichero);
  if (linea_tamanio == NULL) {
    syslog(LOG_ERR, "Error en el sprintf");
    if (linea_tamanio) free(linea_tamanio);
    return SERVER_ERROR;
  }
  strcat(cabecera_respuesta, linea_tamanio);
  if (cabecera_respuesta == NULL) {
    syslog(LOG_ERR, "Error en el strcat");
    if (linea_tamanio) free(linea_tamanio);
    return SERVER_ERROR;
  }

  free(linea_tamanio);
  return OK;
}

int _cabecera_terminar(char *cabecera_respuesta, int *cabecera_length) {
  /* Separamos la cabecera del cuerpo */
  strcat(cabecera_respuesta, "\r\n");
  if (cabecera_respuesta == NULL) {
    syslog(LOG_ERR, "Error en el strcat");
    return SERVER_ERROR;
  }

  *cabecera_length += 2;

  return OK;
}

int _cabecera_anadir_version_html(char *cabecera_respuesta, int *cabecera_length) {

  char *linea_version = NULL;

  linea_version = (char *)calloc(1, MAX_LINEA * sizeof(char));
  if (linea_version == NULL) {
    syslog(LOG_ERR, "Error en el calloc");
    return SERVER_ERROR;
  }

  *cabecera_length += sprintf(linea_version, "%s ", HTTP_RESPONSE_VERSION);
  if (linea_version == NULL) {
    syslog(LOG_ERR, "Error en el sprintf");
    if (linea_version) free(linea_version);
    return SERVER_ERROR;
  }

  strcat(cabecera_respuesta, linea_version);
  if (cabecera_respuesta == NULL) {
    syslog(LOG_ERR, "Error en el strcat");
    if (linea_version) free(linea_version);
    return SERVER_ERROR;
  }

  free(linea_version);
  return OK;
}

int _cabecera_anadir_codigo_respuesta(char *cabecera_respuesta, int *cabecera_length, int codigo) {

  char *linea_codigo_respuesta = NULL;

  linea_codigo_respuesta = (char *)calloc(1, MAX_LINEA * sizeof(char));
  if (linea_codigo_respuesta == NULL) {
    syslog(LOG_ERR, "Error en el calloc");
    return SERVER_ERROR;
  }

  *cabecera_length += sprintf(linea_codigo_respuesta, "%d ", codigo);
  if (linea_codigo_respuesta == NULL) {
    syslog(LOG_ERR, "Error en el sprintf");
    if (linea_codigo_respuesta) free(linea_codigo_respuesta);
    return SERVER_ERROR;
  }

  strcat(cabecera_respuesta, linea_codigo_respuesta);
  if (cabecera_respuesta == NULL) {
    syslog(LOG_ERR, "Error en el strcat");
    if (linea_codigo_respuesta) free(linea_codigo_respuesta);
    return SERVER_ERROR;
  }

  free(linea_codigo_respuesta);
  return OK;
}

int _cabecera_anadir_frase_respuesta(char *cabecera_respuesta, int *cabecera_length, char *frase) {

  char *linea_frase_respuesta = NULL;

  linea_frase_respuesta = (char *)calloc(1, MAX_LINEA * sizeof(char));
  if (linea_frase_respuesta == NULL) {
    syslog(LOG_ERR, "Error en el calloc");
    return SERVER_ERROR;
  }

  *cabecera_length += sprintf(linea_frase_respuesta, "%s\r\n", frase);
  if (linea_frase_respuesta == NULL) {
    syslog(LOG_ERR, "Error en el sprintf");
    if (linea_frase_respuesta) free(linea_frase_respuesta);
    return SERVER_ERROR;
  }

  strcat(cabecera_respuesta, linea_frase_respuesta);
  if (cabecera_respuesta == NULL) {
    syslog(LOG_ERR, "Error en el strcat");
    if (linea_frase_respuesta) free(linea_frase_respuesta);
    return SERVER_ERROR;
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
      syslog(LOG_ERR, "Error en el sendfile");
      return SEND_ERROR;
    }
  }

  *bytes_mandados = mandados;

  return OK;
}

int _responder_bad_request (int connfd, char* resources_path) {

  char *ruta_absoluta = NULL;
  int retorno;
  int fichero_a_mandar_df;

  /* Actualiza los valores con la página de error para los siguientes valores de la cabecera */
  ruta_absoluta = (char *)malloc((strlen(resources_path) + strlen(PAGINA_BAD_REQUEST) + 1) * sizeof(char));
  if (ruta_absoluta == NULL) {
    syslog(LOG_ERR, "Error en el malloc en _responder_bad_request");
    return SERVER_ERROR;
  }

  if (sprintf(ruta_absoluta, "%s%s", resources_path, PAGINA_BAD_REQUEST) < 0) {
    syslog(LOG_ERR, "Error en el sprintf en _responder_bad_request");
    if (ruta_absoluta) free(ruta_absoluta);
    return SERVER_ERROR;
  }

  fichero_a_mandar_df = open(ruta_absoluta, O_RDONLY);
  if (fichero_a_mandar_df < 0) {
    syslog(LOG_ERR, "No se ha encontrado la página para el error 400");
    retorno = _responder_bad_request_sin_ficheros(connfd);

    free(ruta_absoluta);
    return OK;
  }

  retorno = _manda_respuesta_con_fichero(connfd, RESPONSE_BAD_REQUEST_CODE, RESPONSE_BAD_REQUEST_FRASE, ruta_absoluta, fichero_a_mandar_df);
  if (retorno != OK) {
    syslog(LOG_ERR, "Error en _manda_respuesta_con_fichero en _responder_bad_request");
    if (ruta_absoluta) free(ruta_absoluta);
    return SERVER_ERROR;
  }

  close(fichero_a_mandar_df);
  free(ruta_absoluta);

  return OK;
}

int _responder_bad_request_sin_ficheros(int connfd) {

  manda_respuesta_sin_fichero(connfd, RESPONSE_BAD_REQUEST_CODE, RESPONSE_BAD_REQUEST_FRASE);

  return OK;
}

int _responder_service_unavailable (int connfd, char* resources_path) {

  char *ruta_absoluta = NULL;
  int retorno;
  int fichero_a_mandar_df;

  /* Actualiza los valores con la página de error para los siguientes valores de la cabecera */
  ruta_absoluta = (char *)malloc((strlen(resources_path) + strlen(PAGINA_SERVICE_UNAVAILABLE) + 1) * sizeof(char));
  if (ruta_absoluta == NULL) {
    syslog(LOG_ERR, "Error en el malloc en _responder_service_unavailable");
    return SERVER_ERROR;
  }

  if (sprintf(ruta_absoluta, "%s%s", resources_path, PAGINA_SERVICE_UNAVAILABLE) < 0) {
    syslog(LOG_ERR, "Error en el sprintf en _responder_service_unavailable");
    if (ruta_absoluta) free(ruta_absoluta);
    return SERVER_ERROR;
  }

  fichero_a_mandar_df = open(ruta_absoluta, O_RDONLY);
  if (fichero_a_mandar_df < 0) {
    syslog(LOG_ERR, "No se ha encontrado la página para el error 503");
    retorno = _responder_service_unavailable_sin_ficheros(connfd);

    free(ruta_absoluta);
    return OK;
  }

  retorno = _manda_respuesta_con_fichero(connfd, RESPONSE_SERVICE_UNAVAILABLE_CODE, RESPONSE_SERVICE_UNAVAILABLE_FRASE, ruta_absoluta, fichero_a_mandar_df);
  if (retorno != OK) {
    syslog(LOG_ERR, "Error en _manda_respuesta_con_fichero en _responder_service_unavailable");
    if (ruta_absoluta) free(ruta_absoluta);
    return SERVER_ERROR;
  }

  close(fichero_a_mandar_df);

  free(ruta_absoluta);

  return OK;
}

int _responder_service_unavailable_sin_ficheros(int connfd) {

  manda_respuesta_sin_fichero(connfd, RESPONSE_SERVICE_UNAVAILABLE_CODE, RESPONSE_SERVICE_UNAVAILABLE_FRASE);

  return OK;
}

int _responder_server_error (int connfd, char* resources_path) {

  char *ruta_absoluta = NULL;
  int retorno;
  int fichero_a_mandar_df;

  /* Actualiza los valores con la página de server error para los siguientes valores de la cabecera */
  ruta_absoluta = (char *)malloc((strlen(resources_path) + strlen(PAGINA_SERVER_ERROR) + 1) * sizeof(char));
  if (ruta_absoluta == NULL) {
    syslog(LOG_ERR, "Error en el malloc en _responder_server_error");
    return SERVER_ERROR;
  }

  if (sprintf(ruta_absoluta, "%s%s", resources_path, PAGINA_SERVER_ERROR) < 0) {
    syslog(LOG_ERR, "Error en el sprintf en _responder_server_error");
    if (ruta_absoluta) free(ruta_absoluta);
    return SERVER_ERROR;
  }

  fichero_a_mandar_df = open(ruta_absoluta, O_RDONLY);
  if (fichero_a_mandar_df < 0) {
    syslog(LOG_ERR, "No se ha encontrado la página para el error 500");
    retorno = _responder_server_error_sin_ficheros(connfd);

    free(ruta_absoluta);
    return OK;
  }

  retorno = _manda_respuesta_con_fichero(connfd, RESPONSE_SERVER_ERROR_CODE, RESPONSE_SERVER_ERROR_FRASE, ruta_absoluta, fichero_a_mandar_df);
  if (retorno != OK) {
    syslog(LOG_ERR, "Error en _manda_respuesta_con_fichero en _responder_server_error");
    if (ruta_absoluta) free(ruta_absoluta);
    return SERVER_ERROR;
  }

  close(fichero_a_mandar_df);
  free(ruta_absoluta);

  return OK;
}

int _responder_server_error_sin_ficheros(int connfd) {

  manda_respuesta_sin_fichero(connfd, RESPONSE_SERVER_ERROR_CODE, RESPONSE_SERVER_ERROR_FRASE);

  return OK;
}

int _responder_not_found(int connfd, char *resources_path) {

  char *ruta_absoluta = NULL;
  int retorno;
  int fichero_a_mandar_df;

  /* Actualiza los valores con la página de error para los siguientes valores de la cabecera */
  ruta_absoluta = (char *)malloc((strlen(resources_path) + strlen(PAGINA_NOT_FOUND) + 1) * sizeof(char));
  if (ruta_absoluta == NULL) {
    syslog(LOG_ERR, "Error en el malloc en _responder_not_found");
    return SERVER_ERROR;
  }

  if (sprintf(ruta_absoluta, "%s%s", resources_path, PAGINA_NOT_FOUND) < 0) {
    syslog(LOG_ERR, "Error en el sprintf en _responder_not_found");
    if (ruta_absoluta) free(ruta_absoluta);
    return SERVER_ERROR;
  }

  fichero_a_mandar_df = open(ruta_absoluta, O_RDONLY);
  if (fichero_a_mandar_df < 0) {
    /* No se encuentra la página de error */
    syslog(LOG_ERR, "No se ha encontrado la página para el error 404 (%s)", ruta_absoluta);
    retorno = _responder_not_found_sin_ficheros(connfd);

    free(ruta_absoluta);
    return OK;
  }

  retorno = _manda_respuesta_con_fichero(connfd, RESPONSE_NOT_FOUND_CODE, RESPONSE_NOT_FOUND_FRASE, ruta_absoluta, fichero_a_mandar_df);
  if (retorno != OK) {
    syslog(LOG_ERR, "Error en _manda_respuesta_con_fichero en _responder_not_found");
    if (ruta_absoluta) free(ruta_absoluta);
    return SERVER_ERROR;
  }

  free(ruta_absoluta);

  return OK;

}

int _responder_not_found_sin_ficheros(int connfd) {

  manda_respuesta_sin_fichero(connfd, RESPONSE_NOT_FOUND_CODE, RESPONSE_NOT_FOUND_FRASE);

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
    syslog(LOG_ERR, "Error en calloc en _manda_respuesta_con_fichero");
    return SERVER_ERROR;
  }

  /* Forma la cabecera */

  /* 1.1 Escribe la version */
  retorno = _cabecera_anadir_version_html(cabecera_respuesta, &cabecera_length);
  if (retorno != OK) {
    syslog(LOG_ERR, "Error en _cabecera_anadir_version_html en _manda_respuesta_con_fichero");
    if (cabecera_respuesta) free(cabecera_respuesta);
    return SERVER_ERROR;
  }

  /* 1.2 Escribe el codigo de respuesta */
  retorno = _cabecera_anadir_codigo_respuesta(cabecera_respuesta, &cabecera_length, codigo);
  if (retorno != OK) {
    syslog(LOG_ERR, "Error en _cabecera_anadir_codigo_respuesta en _manda_respuesta_con_fichero");
    if (cabecera_respuesta) free(cabecera_respuesta);
    return SERVER_ERROR;
  }

  /* 1.3 Escribe la frase de respuesta */
  retorno = _cabecera_anadir_frase_respuesta(cabecera_respuesta, &cabecera_length, frase);
  if (retorno != OK) {
    syslog(LOG_ERR, "Error en _cabecera_anadir_frase_respuesta en _manda_respuesta_con_fichero");
    if (cabecera_respuesta) free(cabecera_respuesta);
    return SERVER_ERROR;
  }

  /* 2. Escribe los campos de cabecera */

  /* 2.1 Escribe el tipo de fichero */
  retorno = _cabecera_anadir_tipo_fichero(cabecera_respuesta, &cabecera_length, ruta_absoluta);
  if (retorno != OK) {
    syslog(LOG_ERR, "Error en _cabecera_anadir_tipo_fichero en _manda_respuesta_con_fichero");
    if (cabecera_respuesta) free(cabecera_respuesta);
    return SERVER_ERROR;
  }

  /* 2.2 Escribe el tamaño de fichero */
  retorno = _cabecera_anadir_tamanio_fichero(cabecera_respuesta, &cabecera_length, ruta_absoluta, &tamanio_fichero);
  if (retorno != OK) {
    syslog(LOG_ERR, "Error en _cabecera_anadir_tamanio_fichero en _manda_respuesta_con_fichero");
    if (cabecera_respuesta) free(cabecera_respuesta);
    return SERVER_ERROR;
  }

  /* 2.3. Escribe la fecha y hora actuales */
  retorno = _cabecera_anadir_fecha_y_hora_actual(cabecera_respuesta, &cabecera_length);
  if (retorno != OK) {
    syslog(LOG_ERR, "Error en _cabecera_anadir_fecha_y_hora_actual en _manda_respuesta_con_fichero");
    if (cabecera_respuesta) free(cabecera_respuesta);
    return SERVER_ERROR;
  }

  /* 2.4. Escribe la hora de ultima modificacion del fichero */
  retorno = _cabecera_anadir_ultima_modificacion(cabecera_respuesta, &cabecera_length, ruta_absoluta);
  if (retorno != OK) {
    syslog(LOG_ERR, "Error en _cabecera_anadir_ultima_modificacion en _manda_respuesta_con_fichero");
    if (cabecera_respuesta) free(cabecera_respuesta);
    return SERVER_ERROR;
  }

  /* 2.5 Termina la cabecera */
  retorno = _cabecera_terminar(cabecera_respuesta, &cabecera_length);
  if (retorno != OK) {
    syslog(LOG_ERR, "Error en _cabecera_terminar en _manda_respuesta_con_fichero");
    if (cabecera_respuesta) free(cabecera_respuesta);
    return SERVER_ERROR;
  }


  /* 3. Manda la cabecera */
  if (send(connfd, cabecera_respuesta, cabecera_length, 0) < 0) {
    syslog(LOG_ERR, "Error en send en _manda_respuesta_con_fichero");
    if (cabecera_respuesta) free(cabecera_respuesta);
    return SERVER_ERROR;
  }

  /* 4. Manda el archivo */
  retorno = _mandar_fichero_chunks(connfd, fichero_a_mandar_df, tamanio_fichero, TAMANIO_CHUNK, &bytes_mandados);

  if (bytes_mandados < 0) {
    syslog(LOG_ERR, "Error en _mandar_fichero_chunks en _manda_respuesta_con_fichero");
    if (cabecera_respuesta) free(cabecera_respuesta);
    return SERVER_ERROR;
  } else {
    syslog(LOG_INFO, "Archivo mandado: %s (%d bytes)", ruta_absoluta, bytes_mandados);
  }


  if (fichero_a_mandar_df > 0) {
    close(fichero_a_mandar_df);
  }

  free(cabecera_respuesta);

  return OK;

}

int manda_respuesta_sin_fichero(int connfd, int codigo, char *frase) {

  char *cabecera_respuesta = NULL;
  int cabecera_length = 0;
  int tamanio_fichero;
  int retorno;

  /* Reserva memoria para la cabecera de la respuesta */

  cabecera_respuesta = (char *)calloc(1, MAX_CABECERA * sizeof(char));
  if (cabecera_respuesta == NULL) {
    syslog(LOG_ERR, "Error en calloc en manda_respuesta_sin_fichero");
    return SERVER_ERROR;
  }


  /* Forma la cabecera */

  /* 1.1 Escribe la version */
  retorno = _cabecera_anadir_version_html(cabecera_respuesta, &cabecera_length);
  if (retorno != OK) {
    syslog(LOG_ERR, "Error en _cabecera_anadir_version_html en manda_respuesta_sin_fichero");
    if (cabecera_respuesta) free(cabecera_respuesta);
    return SERVER_ERROR;
  }

  /* 1.2 Escribe el codigo de respuesta */
  retorno = _cabecera_anadir_codigo_respuesta(cabecera_respuesta, &cabecera_length, codigo);
  if (retorno != OK) {
    syslog(LOG_ERR, "Error en _cabecera_anadir_codigo_respuesta en manda_respuesta_sin_fichero");
    if (cabecera_respuesta) free(cabecera_respuesta);
    return SERVER_ERROR;
  }

  /* 1.3 Escribe la frase de respuesta */
  retorno = _cabecera_anadir_frase_respuesta(cabecera_respuesta, &cabecera_length, frase);
  if (retorno != OK) {
    syslog(LOG_ERR, "Error en _cabecera_anadir_frase_respuesta en manda_respuesta_sin_fichero");
    if (cabecera_respuesta) free(cabecera_respuesta);
    return SERVER_ERROR;
  }

  /* 2. Escribe los campos de cabecera */

  /* 2.1 Escribe el tipo de fichero --> No se hace */


  /* 2.2 Escribe el tamaño de fichero */
  retorno = _cabecera_anadir_tamanio_fichero(cabecera_respuesta, &cabecera_length, NULL, &tamanio_fichero);
  if (retorno != OK) {
    syslog(LOG_ERR, "Error en _cabecera_anadir_tamanio_fichero en manda_respuesta_sin_fichero");
    if (cabecera_respuesta) free(cabecera_respuesta);
    return SERVER_ERROR;
  }

  /* 2.3. Escribe la fecha y hora actuales */
  retorno = _cabecera_anadir_fecha_y_hora_actual(cabecera_respuesta, &cabecera_length);
  if (retorno != OK) {
    syslog(LOG_ERR, "Error en _cabecera_anadir_fecha_y_hora_actual en manda_respuesta_sin_fichero");
    if (cabecera_respuesta) free(cabecera_respuesta);
    return SERVER_ERROR;
  }

  /* 2.4. Escribe la hora de ultima modificacion del fichero --> No se hace */


  /* 2.5 Termina la cabecera */
  retorno = _cabecera_terminar(cabecera_respuesta, &cabecera_length);
  if (retorno != OK) {
    syslog(LOG_ERR, "Error en _cabecera_terminar en manda_respuesta_sin_fichero");
    if (cabecera_respuesta) free(cabecera_respuesta);
    return SERVER_ERROR;
  }


  /* 3. Manda la cabecera */
  if (send(connfd, cabecera_respuesta, cabecera_length, 0) < 0) {
    syslog(LOG_ERR, "Error en send en manda_respuesta_sin_fichero");
    if (cabecera_respuesta) free(cabecera_respuesta);
    return SERVER_ERROR;
  }


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
    syslog(LOG_ERR, "Error en malloc en _fichero_es_script");
    return SERVER_ERROR;
  }
  strcpy(ruta_auxiliar, ruta);
  if (ruta_auxiliar == NULL) {
    syslog(LOG_ERR, "Error en strcpy en _fichero_es_script");
    return SERVER_ERROR;
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

int _get_ruta_script_y_variables(char *ruta_absoluta, char **ruta_script_p, char **variables_p) {

  char *interrogacion = NULL;
  char *ruta_auxiliar = NULL;
  char *variables_aux = NULL;

  ruta_auxiliar = (char *)malloc((strlen(ruta_absoluta) + 1) * sizeof(char));
  if (ruta_auxiliar == NULL) {
    syslog(LOG_ERR, "Error en malloc en _get_ruta_script_y_variables");
    return SERVER_ERROR;
  }
  strcpy(ruta_auxiliar, ruta_absoluta);
  if (ruta_auxiliar == NULL) {
    syslog(LOG_ERR, "Error en strcpy en _get_ruta_script_y_variables");
    if (ruta_auxiliar) free(ruta_auxiliar);
    return SERVER_ERROR;
  }


  interrogacion = strrchr(ruta_auxiliar, '?');

  if (interrogacion == NULL || interrogacion == ruta_auxiliar) {
    *ruta_script_p = (char *)malloc((strlen(ruta_auxiliar) + 1) * sizeof(char));
    if (*ruta_script_p == NULL) {
      syslog(LOG_ERR, "Error en malloc en _get_ruta_script_y_variables");
      if (ruta_auxiliar) free(ruta_auxiliar);
      return SERVER_ERROR;
    }
    strcpy(*ruta_script_p, ruta_auxiliar);
    if (*ruta_script_p == NULL) {
      syslog(LOG_ERR, "Error en strcpy en _get_ruta_script_y_variables");
      if (ruta_auxiliar) free(ruta_auxiliar);
      if (*ruta_script_p) free(*ruta_script_p);
      return SERVER_ERROR;
    }

    free(ruta_auxiliar);
    return OK;
  } else {
    interrogacion[0] = '\0';

    variables_aux = interrogacion + 1;

    *ruta_script_p = (char *)malloc((strlen(ruta_auxiliar) + 1) * sizeof(char));
    if (*ruta_script_p == NULL) {
      syslog(LOG_ERR, "Error en malloc en _get_ruta_script_y_variables");
      if (ruta_auxiliar) free(ruta_auxiliar);
      if (*ruta_script_p) free(*ruta_script_p);
      return SERVER_ERROR;
    }
    strcpy(*ruta_script_p, ruta_auxiliar);
    if (*ruta_script_p == NULL) {
      syslog(LOG_ERR, "Error en strcpy en _get_ruta_script_y_variables");
      if (ruta_auxiliar) free(ruta_auxiliar);
      if (*ruta_script_p) free(*ruta_script_p);
      return SERVER_ERROR;
    }

    *variables_p = (char *)malloc((strlen(variables_aux) + 1) * sizeof(char));
    if (*variables_p == NULL) {
      syslog(LOG_ERR, "Error en malloc en _get_ruta_script_y_variables");
      if (ruta_auxiliar) free(ruta_auxiliar);
      if (*ruta_script_p) free(*ruta_script_p);
      return SERVER_ERROR;
    }
    strcpy(*variables_p, variables_aux);
    if (*variables_p == NULL) {
      syslog(LOG_ERR, "Error en strcpy en _get_ruta_script_y_variables");
      if (ruta_auxiliar) free(ruta_auxiliar);
      if (*ruta_script_p) free(*ruta_script_p);
      if (*variables_p) free(*variables_p);
      return SERVER_ERROR;
    }


    free(ruta_auxiliar);

    return OK;
  }
}

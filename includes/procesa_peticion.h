#ifndef PROCESA_PETICION_H
/**
 * @file procesa_peticion.h
 * @defgroup Procesa_peticion Procesa petición
 *
 * Esta librería se encarga de procesar cada
 * petición recibida por el servidor, de forma
 * acorde a los ficheros que existan en la
 * ruta resources_path, con posibilidad de
 * detectar errores
 */

#define PROCESA_PETICION_H /*!< Macro de compilación */

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <syslog.h>
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

#define PARSEERROR -2               /*!< Error al parsear argumentos */
#define REQUESTISTOOLONGERROR -3    /*!< Error: petición es demasiado larga */
#define IOERROR -4                  /*!< Error de IO de SO */

#define CLOSE_CONNECTION_REQUEST -5 /*!< El cliente ha pedido cerrar la conexión */


/**
 * @brief Petición parseador
 *
 * @ingroup Procesa_peticion
 *
 * Esta estructura contiene todos los argumentos
 * parseados de una petición HTTP: método (POST,
 * GET, ...), path del fichero pedido, headers, ...
 */
typedef struct Parsear_ {
  char              buf[4096];      /**< Petición como cadena de caracteres */
  char             *body;           /**< Cuerpo de la petición */
  int               bodylen;        /**< Longitud del cuerpo de la petición */
  char             *method;         /**< Método de la petición */
  char             *path;           /**< Path pedida */
  int               pret;           /**< Retorno de la función phr_parse_request() */
  int               minor_version;  /**< Versión HTTP de la petición */
  struct phr_header headers[100];   /**< Estructura con los headers de la petición */
  size_t            buflen;         /**< Logitud de la petición */
  size_t            prevbuflen;     /**< Valor privado usado al hacer receive() */
  size_t            method_len;     /**< Longitud del método de la petición */
  size_t            path_len;       /**< Longitud de la path de la petición */
  size_t            num_headers;    /**< Cantidad de headers de la petición */
  ssize_t           rret;           /**< Retorno del read() al leer del socket */
} Parsear;

/**
 * @brief Función que parsea una petición
 *
 * Esta función es llamada cuando se recibe una nueva
 * petición. De una string completa, separa cada campo
 * en la estructura Parsear creada para tal efecto
 *
 * @ingroup Procesa_peticion
 * @param connfd Descriptor del socket por el mandar la respuesta
 * @param ret Estructura ret donde guardar los parametros
 * @return Código de ejecución
 */
int parsear_peticion(int connfd, Parsear *ret);

/**
 * @brief Función que procesa una petición
 *
 * Esta función analiza los campos parseados y ejecuta
 * la petición dependiendo del método elegido y la url
 * del fichero pedido. También puede ejecutar script .py y .php
 *
 * @ingroup Procesa_peticion
 * @param connfd Descriptor del socket por el mandar la respuesta
 * @param resources_path Ruta al directorio donde están guardados todos los ficheros
 * @param campos_parseados Estructura Parsear con todos los campos parseados
 * @return Código de ejecución
 */
int procesa_peticion (int connfd, char *resources_path, Parsear campos_parseados);

/**
 * @brief Función principal para una petición GET
 *
 * Esta función es llamada cuando se recibe una
 * petición GET
 *
 * @ingroup Procesa_peticion
 * @param ruta_fichero Ruta del fichero de la petición
 * @param resources_path Ruta raíz de la carpeta resources/
 * @param connfd Descriptor del socket por el mandar la respuesta
 * @param bodylen Tamaño del body de la petición
 * @param body Cuerpo de la petición
 * @return Código de ejecución
 */
int funcionalidad_get(char *ruta_fichero, char *resources_path, int connfd, int bodylen, char *body);

/**
 * @brief Función principal para una petición POST
 *
 * Esta función es llamada cuando se recibe una
 * petición POST
 *
 * @ingroup Procesa_peticion
 * @param ruta_fichero Ruta del fichero de la petición
 * @param resources_path Ruta raíz de la carpeta resources/
 * @param connfd Descriptor del socket por el mandar la respuesta
 * @param bodylen Tamaño del body de la petición
 * @param body Cuerpo de la petición
 * @return Código de ejecución
 */
int funcionalidad_post(char *ruta_fichero, char *resources_path, int connfd, int bodylen, char *body);

/**
 * @brief Función principal para una petición OPTIONS
 *
 * Esta función es llamada cuando se recibe una
 * petición OPTIONS
 *
 * @ingroup Procesa_peticion
 * @param connfd Descriptor del socket por el mandar la respuesta
 * @param resources_path Ruta raíz de la carpeta resources/
 * @return Código de ejecución
 */
int funcionalidad_options(int connfd, char *resources_path);

/**
 * @brief Función que ejecuta un script y responde a la petición
 *
 * Esta función ejecuta un script pasándole argumentos
 * y haciendo uno de pipes y ficheros auxiliares
 *
 * @ingroup Procesa_peticion
 * @param connfd Descriptor del socket por el mandar la respuesta
 * @param resources_path Ruta raíz de la carpeta resources/
 * @param ruta_absoluta Ruta del fichero de la petición
 * @param bodylen Tamaño del body de la petición
 * @param body Cuerpo de la petición
 * @return Código de ejecución
 */
int ejecutar_script(int connfd, char* resources_path, char *ruta_absoluta, int bodylen, char *body);

/* Funciones de respuesta */

/**
 * @brief Función que responde con un fichero
 *
 * Esta función forma una respuesta HTTP con un fichero y con
 * respecto a los argumentos especificados y la envía
 *
 * @ingroup Procesa_peticion
 * @param connfd Descriptor de la socket por el que responder
 * @param codigo Código de respuesta HTTP (200, 404....)
 * @param frase Frase de respuesta HTTP (OK, Not Found....)
 * @param ruta_fichero Ruta del fichero a mandar
 * @param fichero_a_mandar_df Descriptor del fichero a mandar
 * @return Código de ejecución
 */
int _manda_respuesta_con_fichero(int connfd, int codigo, char *frase, char *ruta_fichero, int fichero_a_mandar_df);

/**
 * @brief Función que responde sin fichero
 *
 * Esta función forma una respuesta HTTP con respecto
 * a los argumentos especificados y la envía
 *
 * @ingroup Procesa_peticion
 * @param connfd Descriptor de la socket por el que responder
 * @param codigo Código de respuesta HTTP (200, 404....)
 * @param frase Frase de respuesta HTTP (OK, Not Found....)
 * @return Código de ejecución
 */
int manda_respuesta_sin_fichero(int connfd, int codigo, char *frase);

#endif

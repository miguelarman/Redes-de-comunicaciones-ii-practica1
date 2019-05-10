#ifndef CONFIGPARSER_H
/**
 * @file configParser.h
 * @defgroup ConfigParser Configuración de proyecto
 *
 * Contiene la funcion para parsear el fichero de configuracion del servidor.
 */

#define CONFIGPARSER_H /*!< Macro de compilación */

#define DEFAULT_SERVER_ROOT "\\" /*!< Ruta del servidor por defecto */
#define DEFAULT_MAX_CLIENTS 20 /*!< Numero maximo de clientes por defecto */
#define DEFAULT_LISTEN_PORT 8080 /*!< Puerto del servidor */
#define DEFAULT_SERVER_SIGNATURE "Redes2" /*!< Nuestra firma por defecto */
#define DEFAULT_THREAD_COUNT 20 /*!< Numero de threads por defecto */
#define DEFAULT_QUEUE_SIZE 20 /*!< Tamanio de la cola por defecto */

#define MAX_STR 100 /*!< Tamanio maximo de cadena */

#define ERROR -1 /*!< Codigo de error */
#define SUCCESS 0 /*!< Codigo de exito */

/**
 * @brief Estructura que contiene las opciones de configuracion
 *
 * @ingroup ConfigParser
 * configOptions contiene las opciones de configuracion del servidor, parseadas
 * con la libreria confuse
 */
typedef struct
{
  char server_root[MAX_STR]; /**< Ruta del servidor */
  int max_clients; /**< Numero maximo de clientes */
  int listen_port; /**< Puerto del servidor */
  char server_signature[MAX_STR]; /**< Firma del servidor */
  int thread_count; /**< Nummero de threads */
  int queue_size; /**< Tamanio de la cola */
} configOptions;

/**
 * @brief parsea la configuracion del servidor
 *
 * DEsta funcion se encarga de parsear la configuracion del servidor de un
 * fichero .conf
 *
 * @ingroup ConfigParser
 * @param cf ruta del .conf
 * @param out_confopt por aqui se rellena la estructura configOptions
 * @return Devuelve ERROR si ha habido algun problema; SUCCESS en caso contrario
 */
int parseConfig(char *cf, configOptions *out_confopt);

#endif

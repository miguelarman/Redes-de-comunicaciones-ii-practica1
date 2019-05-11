#ifndef CONFIGPARSER_H
/**
 * @file configParser.h
 * @defgroup ConfigParser Configuración de proyecto
 *
 * TODO Descripción del grupo
 */

#define CONFIGPARSER_H /*!< Macro de compilación */

#define DEFAULT_SERVER_ROOT "\\" /*!< TODO Descripción de macro */
#define DEFAULT_MAX_CLIENTS 20 /*!< TODO Descripción de macro */
#define DEFAULT_LISTEN_PORT 8080 /*!< TODO Descripción de macro */
#define DEFAULT_SERVER_SIGNATURE "Redes2" /*!< TODO Descripción de macro */
#define DEFAULT_THREAD_COUNT 20 /*!< TODO Descripción de macro */
#define DEFAULT_QUEUE_SIZE 20 /*!< TODO Descripción de macro */

#define MAX_STR 100 /*!< TODO Descripción de macro */

#define ERROR -1 /*!< TODO Descripción de macro */
#define SUCCESS 0 /*!< TODO Descripción de macro */

/**
 * @brief TODO Descripción de la estructura
 *
 * @ingroup ConfigParser
 * TODO Descripción de la estructura
 */
typedef struct
{
  char server_root[MAX_STR]; /**< TODO Descripción de campo de estructura */
  int max_clients; /**< TODO Descripción de campo de estructura */
  int listen_port; /**< TODO Descripción de campo de estructura */
  char server_signature[MAX_STR]; /**< TODO Descripción de campo de estructura */
  int thread_count; /**< TODO Descripción de campo de estructura */
  int queue_size; /**< TODO Descripción de campo de estructura */
} configOptions;

/**
 * @brief TODO Descripción simple de la función
 *
 * TODO Descripción de la funcion
 *
 * @ingroup ConfigParser
 * @param cf TODO Descripción del argumento
 * @param out_confopt TODO Descripción del argumento
 * @return TODO Descripción del retorno
 */
int parseConfig(char *cf, configOptions *out_confopt);

#endif

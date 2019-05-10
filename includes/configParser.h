#ifndef CONFIGPARSER_H
/**
 * @file configParser.h
 * @defgroup ConfigParser
 *
 * Descripción del grupo
 */

#define CONFIGPARSER_H /*!< Macro de compilación */

#define DEFAULT_SERVER_ROOT "\\" /*!< Descripción de macro */
#define DEFAULT_MAX_CLIENTS 20 /*!< Descripción de macro */
#define DEFAULT_LISTEN_PORT 8080 /*!< Descripción de macro */
#define DEFAULT_SERVER_SIGNATURE "Redes2" /*!< Descripción de macro */
#define DEFAULT_THREAD_COUNT 20 /*!< Descripción de macro */
#define DEFAULT_QUEUE_SIZE 20 /*!< Descripción de macro */

#define MAX_STR 100 /*!< Descripción de macro */

#define ERROR -1 /*!< Descripción de macro */
#define SUCCESS 0 /*!< Descripción de macro */

/**
 * @brief Descripción de la estructura
 *
 * @ingroup ConfigParser
 * Descripción de la estructura
 */
typedef struct
{
  char server_root[MAX_STR]; /**< Descripción de campo de estructura */
  int max_clients; /**< Descripción de campo de estructura */
  int listen_port; /**< Descripción de campo de estructura */
  char server_signature[MAX_STR]; /**< Descripción de campo de estructura */
  int thread_count; /**< Descripción de campo de estructura */
  int queue_size; /**< Descripción de campo de estructura */
} configOptions;

/**
 * @brief Descripción simple de la función
 *
 * Descripción de la funcion
 *
 * @ingroup ConfigParser
 * @param cf Descripción del argumento
 * @param out_confopt Descripción del argumento
 * @return Descripción del retorno
 */
int parseConfig(char *cf, configOptions *out_confopt);

#endif

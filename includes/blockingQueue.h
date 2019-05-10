#ifndef BLOCKINGQUEUE_H
/**
 * @file blockingQueue.h
 * @defgroup BlockingQueue Cola bloqueante
 *
 * Contiene una implementacion de cola bloqueante
 */

#define BLOCKINGQUEUE_H /*!< Macro de compilación */

#define ERROR -1  /*!< Codigo de error */
#define SUCCESS 0 /*!< Codigo de exito */

/**
 * @brief Estructura de la cola bloqueante
 *
 * Estructura de la cola bloqueante
 */
typedef struct _blockingQueue blockingQueue;

/**
 * @brief Funcion constructora de la cola bloqueante
 *
 * Esta funcion crea e inicializa los parametros de la cola bloqueante
 *
 * @ingroup BlockingQueue
 * @param q_out Por aqui se devuele la estructura
 * @param size Tamanio de la cola bloqueante
 * @return ERROR si ha habido un problema; SUCCESS en caso contrario
 */
int blockingQueue_create(blockingQueue **q_out, int size);

/**
 * @brief Funcion destructora de la cola bloqueante
 *
 * Esta funcion destruye y libera la cola bloqueante
 *
 * @ingroup BlockingQueue
 * @param q Cola que se quiere destruir
 * @return ERROR si ha habido un problema; SUCCESS en caso contrario
 */
int blockingQueue_destroy(blockingQueue *q);

/**
 * @brief Funcion para meter un elemento en la cola
 *
 * Introduce un elemento en la cola
 *
 * @ingroup BlockingQueue
 * @param q Cola en la que se quiere meter el elemento
 * @param entry Elemento que se quiere meter en la cola
 * @return ERROR si ha habido un problema; SUCCESS en caso contrario
 */
int blockingQueue_put(blockingQueue *q, void *entry);

/**
 * @brief Funcion para sacar un elemento de la cola
 *
 * Saca un elemento de la cola
 *
 * @ingroup BlockingQueue
 * @param q Cola de la que se quiere sacar el elemento
 * @param entry_out Por aqui se devuelve el elemento
 * @return ERROR si ha habido un problema; SUCCESS en caso contrario
 */
int blockingQueue_get(blockingQueue *q, void **entry_out);

#endif

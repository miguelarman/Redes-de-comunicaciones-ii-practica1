#ifndef BLOCKINGQUEUE_H
/**
 * @file blockingQueue.h
 * @defgroup BlockingQueue
 *
 * Descripción del grupo
 */

#define BLOCKINGQUEUE_H /*!< Macro de compilación */

#define ERROR -1  /*!< Descripción de macro */
#define SUCCESS 0 /*!< Descripción de macro */

/**
 * @brief Descripción de la estructura
 *
 * Descripción de la estructura
 */
typedef struct _blockingQueue blockingQueue;

/**
 * @brief Descripción simple de la función
 *
 * Descripción de la funcion
 *
 * @ingroup BlockingQueue
 * @param q_out Descripción del argumento
 * @param size Descripción del argumento
 * @return Descripción del retorno
 */
int blockingQueue_create(blockingQueue **q_out, int size);

/**
 * @brief Descripción simple de la función
 *
 * Descripción de la funcion
 *
 * @ingroup BlockingQueue
 * @param q Descripción del argumento
 * @return Descripción del retorno
 */
int blockingQueue_destroy(blockingQueue *q);

/**
 * @brief Descripción simple de la función
 *
 * Descripción de la funcion
 *
 * @ingroup BlockingQueue
 * @param q Descripción del argumento
 * @param entry Descripción del argumento
 * @return Descripción del retorno
 */
int blockingQueue_put(blockingQueue *q, void *entry);

/**
 * @brief Descripción simple de la función
 *
 * Descripción de la funcion
 *
 * @ingroup BlockingQueue
 * @param q Descripción del argumento
 * @param entry_out Descripción del argumento
 * @return Descripción del retorno
 */
int blockingQueue_get(blockingQueue *q, void **entry_out);

#endif

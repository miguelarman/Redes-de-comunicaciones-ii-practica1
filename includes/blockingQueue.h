#ifndef BLOCKINGQUEUE_H
/**
 * @file blockingQueue.h
 * @defgroup BlockingQueue Cola bloqueante
 *
 * TODO Descripción del grupo
 */

#define BLOCKINGQUEUE_H /*!< Macro de compilación */

#define ERROR -1  /*!< TODO Descripción de macro */
#define SUCCESS 0 /*!< TODO Descripción de macro */

/**
 * @brief TODO Descripción de la estructura
 *
 * TODO Descripción de la estructura
 */
typedef struct _blockingQueue blockingQueue;

/**
 * @brief TODO Descripción simple de la función
 *
 * TODO Descripción de la funcion
 *
 * @ingroup BlockingQueue
 * @param q_out TODO Descripción del argumento
 * @param size TODO Descripción del argumento
 * @return TODO Descripción del retorno
 */
int blockingQueue_create(blockingQueue **q_out, int size);

/**
 * @brief TODO Descripción simple de la función
 *
 * TODO Descripción de la funcion
 *
 * @ingroup BlockingQueue
 * @param q TODO Descripción del argumento
 * @return TODO Descripción del retorno
 */
int blockingQueue_destroy(blockingQueue *q);

/**
 * @brief TODO Descripción simple de la función
 *
 * TODO Descripción de la funcion
 *
 * @ingroup BlockingQueue
 * @param q TODO Descripción del argumento
 * @param entry TODO Descripción del argumento
 * @return TODO Descripción del retorno
 */
int blockingQueue_put(blockingQueue *q, void *entry);

/**
 * @brief TODO Descripción simple de la función
 *
 * TODO Descripción de la funcion
 *
 * @ingroup BlockingQueue
 * @param q TODO Descripción del argumento
 * @param entry_out TODO Descripción del argumento
 * @return TODO Descripción del retorno
 */
int blockingQueue_get(blockingQueue *q, void **entry_out);

#endif

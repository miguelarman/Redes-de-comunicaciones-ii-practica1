#ifndef BLOCKINGQUEUE_H
/**
 * @file blockingQueue.h
 * @defgroup BlockingQueue Cola bloqueante
 *
<<<<<<< HEAD
 * Contiene una implementacion de cola bloqueante
=======
 * TODO Descripción del grupo
>>>>>>> e9607e34cb2230c577491f62e5de91dea86fbe46
 */

#define BLOCKINGQUEUE_H /*!< Macro de compilación */

<<<<<<< HEAD
#define ERROR -1  /*!< Codigo de error */
#define SUCCESS 0 /*!< Codigo de exito */

/**
 * @brief Estructura de la cola bloqueante
 *
 * Estructura de la cola bloqueante
=======
#define ERROR -1  /*!< TODO Descripción de macro */
#define SUCCESS 0 /*!< TODO Descripción de macro */

/**
 * @brief TODO Descripción de la estructura
 *
 * TODO Descripción de la estructura
>>>>>>> e9607e34cb2230c577491f62e5de91dea86fbe46
 */
typedef struct _blockingQueue blockingQueue;

/**
<<<<<<< HEAD
 * @brief Funcion constructora de la cola bloqueante
 *
 * Esta funcion crea e inicializa los parametros de la cola bloqueante
 *
 * @ingroup BlockingQueue
 * @param q_out Por aqui se devuele la estructura
 * @param size Tamanio de la cola bloqueante
 * @return ERROR si ha habido un problema; SUCCESS en caso contrario
=======
 * @brief TODO Descripción simple de la función
 *
 * TODO Descripción de la funcion
 *
 * @ingroup BlockingQueue
 * @param q_out TODO Descripción del argumento
 * @param size TODO Descripción del argumento
 * @return TODO Descripción del retorno
>>>>>>> e9607e34cb2230c577491f62e5de91dea86fbe46
 */
int blockingQueue_create(blockingQueue **q_out, int size);

/**
<<<<<<< HEAD
 * @brief Funcion destructora de la cola bloqueante
 *
 * Esta funcion destruye y libera la cola bloqueante
 *
 * @ingroup BlockingQueue
 * @param q Cola que se quiere destruir
 * @return ERROR si ha habido un problema; SUCCESS en caso contrario
=======
 * @brief TODO Descripción simple de la función
 *
 * TODO Descripción de la funcion
 *
 * @ingroup BlockingQueue
 * @param q TODO Descripción del argumento
 * @return TODO Descripción del retorno
>>>>>>> e9607e34cb2230c577491f62e5de91dea86fbe46
 */
int blockingQueue_destroy(blockingQueue *q);

/**
<<<<<<< HEAD
 * @brief Funcion para meter un elemento en la cola
 *
 * Introduce un elemento en la cola
 *
 * @ingroup BlockingQueue
 * @param q Cola en la que se quiere meter el elemento
 * @param entry Elemento que se quiere meter en la cola
 * @return ERROR si ha habido un problema; SUCCESS en caso contrario
=======
 * @brief TODO Descripción simple de la función
 *
 * TODO Descripción de la funcion
 *
 * @ingroup BlockingQueue
 * @param q TODO Descripción del argumento
 * @param entry TODO Descripción del argumento
 * @return TODO Descripción del retorno
>>>>>>> e9607e34cb2230c577491f62e5de91dea86fbe46
 */
int blockingQueue_put(blockingQueue *q, void *entry);

/**
<<<<<<< HEAD
 * @brief Funcion para sacar un elemento de la cola
 *
 * Saca un elemento de la cola
 *
 * @ingroup BlockingQueue
 * @param q Cola de la que se quiere sacar el elemento
 * @param entry_out Por aqui se devuelve el elemento
 * @return ERROR si ha habido un problema; SUCCESS en caso contrario
=======
 * @brief TODO Descripción simple de la función
 *
 * TODO Descripción de la funcion
 *
 * @ingroup BlockingQueue
 * @param q TODO Descripción del argumento
 * @param entry_out TODO Descripción del argumento
 * @return TODO Descripción del retorno
>>>>>>> e9607e34cb2230c577491f62e5de91dea86fbe46
 */
int blockingQueue_get(blockingQueue *q, void **entry_out);

#endif

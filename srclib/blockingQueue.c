/**
<<<<<<< HEAD
 * @brief Contiene una implementacion de cola bloqueante
 *
 * Cotiene una implementacion de cola bloqueante
=======
 * @brief TODO Descripción del fichero/librería
 *
 * TODO Descripción más elaborada
>>>>>>> e9607e34cb2230c577491f62e5de91dea86fbe46
 *
 * @file blockingQueue.c
 * @author Miguel Arconada Manteca y Mario García Pascual
 * @date 9-5-2019
 */

#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#include "../includes/blockingQueue.h"

/**
<<<<<<< HEAD
 * @brief Estructura de la cola bloqueante
 *
 * @ingroup BlockingQueue
 * Estructura de la cola bloqueante
 */
struct _blockingQueue {
    int size; /**< Tamanio de la cola */
    int count; /**< Numero de elementos que hay en la cola */
    int head; /**< Head de la cola */
    int tail; /**< Tail de la cola */
    void **entries; /**< Array que contiene los elementos de la cola */
    pthread_mutex_t lock; /**< Mutex de la cola */
    sem_t filled; /**< Semaforo de slots llenos */
    sem_t empty; /**< Semaforo de slots vacios */
=======
 * @brief TODO Descripción de la estructura
 *
 * @ingroup BlockingQueue
 * TODO Descripción de la estructura
 */
struct _blockingQueue {
    int size; /**< TODO Descripción de campo de estructura */
    int count; /**< TODO Descripción de campo de estructura */
    int head; /**< TODO Descripción de campo de estructura */
    int tail; /**< TODO Descripción de campo de estructura */
    void **entries; /**< TODO Descripción de campo de estructura */
    pthread_mutex_t lock; /**< TODO Descripción de campo de estructura */
    sem_t filled; /**< TODO Descripción de campo de estructura */
    sem_t empty; /**< TODO Descripción de campo de estructura */
>>>>>>> e9607e34cb2230c577491f62e5de91dea86fbe46
};

int blockingQueue_create(blockingQueue **q_out, int size) {
    blockingQueue *q;

    if (q_out == NULL) {
      return ERROR;
    }

    if (size <= 0) {
        return ERROR;
    }
    q = (blockingQueue *)malloc(sizeof(blockingQueue));
    q->size = size;
    q->count = 0;
    q->head = 0;
    q->tail = 0;
    q->entries = (void **)malloc(sizeof(void *) * size);
    pthread_mutex_init(&q->lock, NULL);
    sem_init(&q->filled, 0, 0);
    sem_init(&q->empty, 0, size);
    *q_out = q;
    return SUCCESS;
}

int blockingQueue_destroy(blockingQueue *q) {
    if (q == NULL) {
        return ERROR;
    }
    free(q->entries);
    pthread_mutex_destroy(&q->lock);
    sem_destroy(&q->filled);
    sem_destroy(&q->empty);
    free(q);
    return SUCCESS;
}

int blockingQueue_get(blockingQueue *q, void **entry_out) {
    if (q == NULL || entry_out == NULL) {
        return ERROR;
    }
    sem_wait(&q->filled);
    /* inicio de seccion critica */
    pthread_mutex_lock(&q->lock);
    *entry_out = q->entries[q->tail++];
    if (q->tail == q->size) q->tail = 0;
    pthread_mutex_unlock(&q->lock);
    /* fin de seccion critica */
    sem_post(&q->empty);
    return SUCCESS;
}

int blockingQueue_put(blockingQueue *q, void *entry) {
    if (q == NULL || entry == NULL) {
        return ERROR;
    }
    sem_wait(&q->empty);
    /* inicio de seccion critica */
    pthread_mutex_lock(&q->lock);
    q->entries[q->head++] = entry;
    if (q->head == q->size) q->head = 0;
    pthread_mutex_unlock(&q->lock);
    /* fin de seccion critica */
    sem_post(&q->filled);
    return SUCCESS;
}

/**
 * @brief Contiene una implementacion de cola bloqueante
 *
 * Cotiene una implementacion de cola bloqueante
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

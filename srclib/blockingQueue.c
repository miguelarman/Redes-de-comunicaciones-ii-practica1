#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#include "blockingQueue.h"

struct _blockingQueue {
    int size;
    int count;
    int head;
    int tail;
    void **entries;
    pthread_mutex_t lock;
    sem_t filled;
    sem_t empty;
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
    // falta control
    q->size = size;
    q->count = 0;
    q->head = 0;
    q->tail = 0;
    q->entries = (void **)malloc(sizeof(void *) * size);
    // falta control
    pthread_mutex_init(&q->lock, NULL);
    // falta control
    sem_init(&q->filled, 0, 0);
    // falta control
    sem_init(&q->empty, 0, size);
    // falta control
    *q_out = q;
    return SUCCESS;
}

// cuando se llame a esta funcion se tiene que asegurar de que
// la cola esta vacia y no hay hilos esperando en los semaforos,
// esto lo hacemos a nivel de threadPool mediante envenenamiento
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
    // inicio de seccion critica
    pthread_mutex_lock(&q->lock);
    *entry_out = q->entries[q->tail++];
    if (q->tail == q->size) q->tail = 0;
    pthread_mutex_unlock(&q->lock);
    // fin de seccion critica
    sem_post(&q->empty);
    return SUCCESS;
}

int blockingQueue_put(blockingQueue *q, void *entry) {
    if (q == NULL || entry == NULL) {
        return ERROR;
    }
    sem_wait(&q->empty);
    // inicio de seccion critica
    pthread_mutex_lock(&q->lock);
    q->entries[q->head++] = entry;
    if (q->head == q->size) q->head = 0;
    pthread_mutex_unlock(&q->lock);
    // fin de seccion critica
    sem_post(&q->filled);
    return SUCCESS;
}


#ifndef BLOCKINGQUEUE_H
#define BLOCKINGQUEUE_H

#define ERROR -1
#define SUCCESS 0

typedef struct _blockingQueue blockingQueue;

int blockingQueue_create(blockingQueue **q_out, int size);

int blockingQueue_destroy(blockingQueue *q);

int blockingQueue_put(blockingQueue *q, void *entry);

int blockingQueue_get(blockingQueue *q, void **entry_out);

#endif

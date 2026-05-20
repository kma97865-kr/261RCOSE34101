#ifndef QUEUE_H
#define QUEUE_H
#include <stdbool.h>
#include "process.h"

//queue stores Process(struct _process *)
#define Item Process

typedef struct _queue *Queue;

Queue queue_create(void);
void queue_destroy(Queue *pq); //double pointer
void queue_make_empty(Queue q);
bool queue_is_empty(Queue q);
bool queue_is_full(Queue q);
void queue_push(Queue q, Item i);
Item queue_pop(Queue q);

//
void queue_show(Queue q);

#endif
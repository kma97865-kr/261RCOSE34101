#ifndef QUEUE_H
#define QUEUE_H
#include <stdbool.h>
#include "process.h"

//queue stores Node(struct node *)
//Node stores Process(struct _process *)

typedef struct _queue *Queue;
typedef struct node *Node;

//Initialize
Queue queue_create(void);
void queue_destroy(Queue *pq); //double pointer
void queue_make_empty(Queue q);
bool queue_is_empty(Queue q);
bool queue_is_full(Queue q);
void queue_push(Queue q, Process i);
Process queue_pop(Queue q);

//Debug
void queue_show(Queue q);

//GETTER
Process queue_front_process(Queue q);
Node queue_front_node(Queue q);

Process queue_node_get_data(Node n);
Node queue_node_next_node(Node n);
int queue_size(Queue q);

//for job_queue.c, SETTER
void queue_node_set_next(Node n1, Node n2);
void queue_set_front(Queue q, Node n);
void queue_set_rear(Queue q, Node n);

//Priority queue : use priority
void priority_queue_push(Queue q, Process i);

//wait queue : use io end time as priority
void wait_queue_push(Queue q, Process i);

//Others
void queue_copy(Queue sq, Queue dq);
#endif
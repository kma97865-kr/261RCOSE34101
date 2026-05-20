#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "process.h" //process 해제의 책임을 어디서 질 것인가? -> 큐를 비울 때 프로세스 해제
#include "queue.h"

//Linked list
struct node{
    Item data;
    struct node *next;
};

struct _queue{ 
    struct node *front; //points to first node
    struct node *rear; //points to last node
};
//typedef struct _queue *Queue;

Queue queue_create(void){
    Queue q = malloc(sizeof(struct _queue));
    if(q == NULL){
        printf("queue_create failure");
        exit(EXIT_FAILURE);
    }
    q->front = NULL;
    q->rear = NULL;
    return q;
}
void queue_destroy(Queue *pq){
    queue_make_empty(*pq);
    free(*pq);
    *pq = NULL;
}
void queue_make_empty(Queue q){
    while(queue_is_empty(q) == false){
        Item temp = queue_pop(q);
        process_destroy(temp);
    }
}
bool queue_is_empty(Queue q){
    return q->front == NULL;
}
bool queue_is_full(Queue q){
    return false;
}
void queue_push(Queue q, Item i){
    struct node *new_node = malloc(sizeof(struct node));
    if(new_node == NULL){
        printf("node create failure");
        exit(EXIT_FAILURE);
    }
    new_node->data = i;
    new_node->next = NULL;

    if(queue_is_empty(q)){
        q->front = new_node;
        q->rear = new_node;
    }
    else{
        q->rear->next = new_node;
        q->rear = new_node;
    }
}
Item queue_pop(Queue q){
    if(queue_is_empty(q)){
        printf("Queue error : queue is empty");
        exit(EXIT_FAILURE);
    }
    struct node *temp = q->front;
    Item itemp = temp->data;

    q->front = q->front->next;
    if(q->front == NULL) q->rear = NULL; //empty

    free(temp);
    return itemp;
}

void queue_show(Queue q){
    if(q == NULL){
        return;
    }
    struct node *now_node = q->front;
    while(now_node != NULL){
        process_show(now_node -> data);
        now_node = now_node -> next;
    }
}
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "process.h" //process 해제의 책임을 어디서 질 것인가? -> 큐를 비울 때 프로세스 해제
#include "queue.h"

//Linked list
struct node{
    Process data;
    struct node *next;
};

struct _queue{ 
    struct node *front; //points to first node
    struct node *rear; //points to last node
    int count;
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
    q->count = 0;
    return q;
}
void queue_destroy(Queue *pq){
    queue_make_empty(*pq);
    free(*pq);
    *pq = NULL;
}
void queue_make_empty(Queue q){
    while(queue_is_empty(q) == false){
        Process temp = queue_pop(q);
        process_destroy(temp);
    }
}
bool queue_is_empty(Queue q){
    if(q == NULL) return true;
    return q->front == NULL;
}
bool queue_is_full(Queue q){
    return false;
}
void queue_push(Queue q, Process i){
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
    q->count++;
}
Process queue_pop(Queue q){
    if(queue_is_empty(q)){
        printf("Queue error : queue is empty");
        exit(EXIT_FAILURE);
    }
    struct node *temp = q->front;
    Process itemp = temp->data;

    q->front = q->front->next;
    if(q->front == NULL) q->rear = NULL; //empty

    free(temp);

    q->count--;
    return itemp;
}

//
Process queue_front_process(Queue q){
    if(queue_is_empty(q)) return NULL;
    return q->front->data;
}

int queue_count(Queue q){
    if(queue_is_empty(q)) return 0;
    return q->count;
}
Node queue_front_node(Queue q){
    if(queue_is_empty(q)) return NULL;
    return q->front;
}

//
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

//
Process queue_node_get_data(Node n){
    if(n==NULL) return NULL;
    return n->data;
}
Node queue_node_next_node(Node n){
    if(n == NULL) return NULL;
    return n->next;
}
int queue_size(Queue q){
    if(q == NULL) return 0;
    return q->count;
}

//
void queue_node_set_next(Node n1, Node n2){
    if(n1 == NULL) return;
    n1 -> next = n2;
}
void queue_set_front(Queue q, Node n){
    q->front = n;
}
void queue_set_rear(Queue q, Node n){
    q->rear = n;
}

//priority_queue
void priority_queue_push(Queue q, Process i){
    //find original seat for Process i
    Node new_node = malloc(sizeof(struct node));
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
        //find its seat
        Node prior_node = NULL;
        Node cur_node = q->front;
        Process temp_p;
        while(cur_node != NULL){
            temp_p = cur_node->data;
            if(process_priority(i) < process_priority(temp_p)){//new_node를 prior_node 와 cur_node 사이에
                if(prior_node == NULL){//cur node가 맨 앞 노드 였을 경우
                    q->front = new_node;
                    new_node->next = cur_node;
                }else{
                    prior_node -> next = new_node;
                    new_node->next = cur_node;
                }
                break;
            }
            //around the queue
            prior_node = cur_node;
            cur_node = cur_node->next;
        }

        if(cur_node == NULL){ //가장 끝에 new_node 붙여야 하는 경우
            q->rear->next = new_node;
            q->rear = new_node;
        }
    }
    q->count++;
}
void wait_queue_push(Queue q, Process i){ //use io end time as priority
    //find original seat for Process i
    Node new_node = malloc(sizeof(struct node));
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
        //find its seat
        Node prior_node = NULL;
        Node cur_node = q->front;
        Process temp_p;
        while(cur_node != NULL){
            temp_p = cur_node->data;
            //only difference with priority queue
            if(process_io_end_time(i) < process_io_end_time(temp_p)){//new_node를 prior_node 와 cur_node 사이에
                if(prior_node == NULL){//cur node가 맨 앞 노드 였을 경우
                    q->front = new_node;
                    new_node->next = cur_node;
                }else{
                    prior_node -> next = new_node;
                    new_node->next = cur_node;
                }
                break;
            }
            //around the queue
            prior_node = cur_node;
            cur_node = cur_node->next;
        }

        if(cur_node == NULL){ //가장 끝에 new_node 붙여야 하는 경우
            q->rear->next = new_node;
            q->rear = new_node;
        }
    }
    q->count++;
}
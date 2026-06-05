#ifndef EVALUATE_H
#define EVALUATE_H
#include "process.h"
#include "queue.h"

extern Queue source_job_queue; 
extern Queue job_queue;
extern Queue ready_queue;
extern Queue wait_queue;
extern Queue terminate_queue;
extern Process running_process;

int evaluate_waiting_time(Queue q, int *);
//return average waiting time
//int[i] : waiting time of proess index i
//int arr의 할당, 해제 책임은 caller가 짐

int evaluate_turnaround_time(Queue q, int *);
//return averave turnaround time
//int[i] : waiting time of proess index i
//int arr의 할당, 해제 책임은 caller가 짐

void evaluate_gantt_chart(Queue q);
//draw gantt chart

void evaluate_algorithm();

#endif
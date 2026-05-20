#ifndef JOB_QUEUE_H
#define JOB_QUEUE_H

#include "queue.h"
#include "process.h"

extern Queue job_queue;

//arrival time 대로 정렬->qsort함수 사용
void job_queue_sort(void);

//time 시간을 받고, 큐를 돌면서 : arrival time이 된 애들을 pop
Process job_queue_to_ready(int time);


#endif
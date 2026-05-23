#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "process.h"
#include "queue.h"
#include "config.h"

/*extern Queue job_queue;
extern Queue ready_queue;
extern Queue wait_queue;
extern Queue terminate_queue;
extern Process runnning_process;*/

extern int p_index;

void config_initialize(void){
    job_queue = queue_create();
    ready_queue = queue_create();
    wait_queue = queue_create();
    terminate_queue = queue_create();
} //create extern values


void config_create_process(int n){
    p_index = 0;
    for(int i = 0; i<n; i++){
        Process temp = process_create();
        queue_push(job_queue, temp);
    }
} //create n process and push into job queue

void config_create_test_process(){
    int arrival[4] = {0, 1,4,6};
    int burst[4] = {5,5, 3,4};
    int priority[4] = {2, 3, 1, 4};
    for(int i = 0; i<4; i++){
        Process temp = process_create();
        process_set_arrival_time(temp, arrival[i]);
        process_set_cpu_burst_time(temp, burst[i]);
        process_set_priority(temp, priority[i]);
        queue_push(job_queue, temp);
    }
}


void config_clean(){
    queue_make_empty(job_queue);
    queue_make_empty(ready_queue);
    queue_make_empty(wait_queue);
    queue_make_empty(terminate_queue);

    queue_destroy(&job_queue);
    queue_destroy(&ready_queue);
    queue_destroy(&wait_queue);
    queue_destroy(&terminate_queue);
}//deallocate all -> destroy extern values
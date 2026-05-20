#include <stdio.h>
#include <stdlib.h>
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
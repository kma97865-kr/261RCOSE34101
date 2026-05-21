#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "process.h"
#include "queue.h"
#include "job_queue.h"
#include "running_process.h"
#include "sch_algorithm.h"

/*extern Queue job_queue; arrival_time 대로 정렬
extern Queue ready_queue; 비어있음
extern Queue wait_queue; 비어있음(현재 사용하지 않음)
extern Queue terminate_queue; 비어있음
extern Process running_process; 비어있음 */ 

void first_come_first_served(){
    int time = -1;
    while(true){
        //escape condition : every proess is terminated, stored in terminate queue
        if(queue_is_empty(job_queue) && queue_is_empty(ready_queue) 
            && queue_is_empty(wait_queue) && (running_process == NULL)){
                break;
        }

        time++;
        //printf("time is %d\n", time);

        //job queue -> ready queue
        Process temp_from_job = job_queue_to_ready(time);
        while(temp_from_job != NULL){
            queue_push(ready_queue, temp_from_job);
            temp_from_job = job_queue_to_ready(time);
        }

        //check running process
        if(running_process != NULL){ //check whether running_process exits
            int remain_time = running_process_time_consume(1);
            if(remain_time > 0){ //still running
                continue;//do nothing, pass to next time
            }else if(remain_time == -1){ //error on consuming
                printf("remain_time failure");
                exit(EXIT_FAILURE);
            }else if(remain_time == 0){//running_process terminated
                process_set_end_time(running_process, (unsigned int)time);
                queue_push(terminate_queue, running_process);
                running_process = NULL;
            }
        }
        else{//running_process is empty
            //do nothing
        }
        

        if(queue_is_empty(ready_queue)) continue;
        else{
            Process temp_p = queue_pop(ready_queue);
            process_set_start_time(temp_p, (unsigned int) time);
            running_process = temp_p;
        }

        
    }
}
/*extern Queue job_queue; 비어있음
extern Queue ready_queue; 비어있음
extern Queue wait_queue; 비어있음(현재 사용하지 않음)
extern Queue terminate_queue; 실행할 프로세스 순으로 정렬(현재는 나누어져 있지 않음)
extern Process running_process; 비어있음 */ 
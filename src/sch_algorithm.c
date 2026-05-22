#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "process.h"
#include "queue.h"
#include "job_queue.h"
#include "running_process.h"
#include "sch_algorithm.h"

//static 함수 : 이 소스코드 외에서 사용하지 않음
static void check_ready_put_running(int time, int *consumed_time){
    if(queue_is_empty(ready_queue)) return; //check ready
        else{ //put runnning
            Process temp_p = queue_pop(ready_queue);
            process_set_start_time(temp_p, (unsigned int) time);
            running_process = temp_p;
            *consumed_time = 0;
        }
}
static int check_priority_preemption(){
    int p1 = process_priority(running_process);

    //ready queue가 비어있는지 확인해야 함.
    if(queue_is_empty(ready_queue)) return 0;
    int p2 = process_priority(queue_front_process(ready_queue));
    return (p1 - p2 > 0) ? 1 : 0;
}
static void setting_sjf(){ //job_queue 순회하면서 priority를 burst time으로 바꿔줌
    Node temp_node = queue_front_node(job_queue);
    Process temp_p;
    while(temp_node != NULL){
        temp_p = queue_node_get_data(temp_node);
        process_set_priority(temp_p, process_cpu_burst_time(temp_p));
        temp_node = queue_node_next_node(temp_node);
    }
}

/*extern Queue job_queue; arrival_time 대로 정렬
extern Queue ready_queue; 비어있음
extern Queue wait_queue; 비어있음(현재 사용하지 않음)
extern Queue terminate_queue; 비어있음
extern Process running_process; 비어있음 */ 

//BASIC FCFS
/*int time = -1;
    while(true){
        //escape condition : every proess is terminated, stored in terminate queue
        if(queue_is_empty(job_queue) && queue_is_empty(ready_queue) 
            && queue_is_empty(wait_queue) && (running_process == NULL)){
                break;
        }

        time++;

        //job queue -> ready queue
        Process temp_from_job = job_queue_to_ready(time);
        while(temp_from_job != NULL){
            queue_push(ready_queue, temp_from_job);
            temp_from_job = job_queue_to_ready(time);
        }

        //check running process
        //assumption : inital burst time of every process > 0
        if(running_process != NULL){ //running_process exits
            int remain_time = running_process_time_consume(1);
            //refresh running_process priority when sjf
            if(sjf) process_set_priority(running_process, process_cpu_burst_time(running_process));

            if(remain_time > 0){ //still running
                continue;
            }else if(remain_time == -1){ //error on consuming
                printf("remain_time failure\n");
                exit(EXIT_FAILURE);
            }else if(remain_time == 0){//running_process terminated
                process_set_end_time(running_process, (unsigned int)time);
                queue_push(terminate_queue, running_process);
                running_process = NULL;
                //
                check_ready_put_running(time);
            }
        }
        else{//running_process is empty
            check_ready_put_running(time);
        }
}*/

void scheduling_algorithm(bool priority, bool preemption, bool sjf, int time_slice){ //
    //handling rule
    if(sjf == true) priority = true;
    if(time_slice < 0){
        printf("Time slice initalizion error\n");
        exit(EXIT_FAILURE);
    }

    int time = -1;
    int consumed_time = 0; //RR

    bool is_pr;
    bool is_rr;

    while(true){
        //escape condition : every proess is terminated, stored in terminate queue
        if(queue_is_empty(job_queue) && queue_is_empty(ready_queue) 
            && queue_is_empty(wait_queue) && (running_process == NULL)){
                break;
        }

        time++;

        //job queue -> ready queue
        Process temp_from_job = job_queue_to_ready(time);
        while(temp_from_job != NULL){
            if(priority) priority_queue_push(ready_queue, temp_from_job); //use priority queue
            else queue_push(ready_queue, temp_from_job); //use ordinary queue (FCFS)
            temp_from_job = job_queue_to_ready(time);
        }
        //wait queue -> ready queue

        //check running process
        //assumption : inital burst time of every process > 0
        if(running_process != NULL){//running_process exits
            int remain_time = running_process_time_consume(1);

            //preemptive sjf has to refreash priority for every time
            if(preemption && sjf) process_set_priority(running_process, process_cpu_burst_time(running_process));

            if(remain_time > 0){ //still running
                consumed_time++;
                //I/O interrupt code

                is_rr = time_slice && (consumed_time >= time_slice); //round_robin check
                is_pr = preemption && check_priority_preemption(); //preemptive check
                if(is_rr || is_pr){ //preemptive code
                    //preemption true -> priority (might) true

                    //termination에 넣을 프로세스 생성 후 push
                    Process new_ter = process_create();
                    process_copy(running_process, new_ter);
                    process_set_end_time(new_ter, (unsigned int)time);
                    queue_push(terminate_queue, new_ter);
                    
                    //현재 running process를 ready_queue에 돌려놓음
                    if(sjf) process_set_priority(running_process, process_cpu_burst_time(running_process));
                    if(priority) priority_queue_push(ready_queue, running_process);
                    else queue_push(ready_queue, running_process);

                    //preemption 진행
                    running_process = NULL;
                    check_ready_put_running(time, &consumed_time);
                }else{//no preemption or round robin
                    continue;//do nothing, pass to next time
                }
            }else if(remain_time == 0){//running_process terminated
                process_set_end_time(running_process, (unsigned int)time);
                queue_push(terminate_queue, running_process);
                running_process = NULL;
                //
                check_ready_put_running(time, &consumed_time);
            }
        }
        else{//running_process is empty
            check_ready_put_running(time, &consumed_time);
        }
    }
}
/*extern Queue job_queue; 비어있음
extern Queue ready_queue; 비어있음
extern Queue wait_queue; 비어있음(현재 사용하지 않음)
extern Queue terminate_queue; 실행할 프로세스 순으로 정렬(현재는 나누어져 있지 않음)
extern Process running_process; 비어있음 */ 


void first_come_first_served(){
    scheduling_algorithm(false, false, false, 0);
}

void non_preemptive_priority(){
    scheduling_algorithm(true, false, false, 0);
}

void non_preemptive_sjf(){
    //priority 갱신의 책임은 non_preemptive_sjf에서 짐
    setting_sjf();
    scheduling_algorithm(true, false, true, 0);
}


void preemptive_priority(){
    scheduling_algorithm(true, true, false, 0);
}

void preemptive_sjf(){
    setting_sjf();
    scheduling_algorithm(true, true, true, 0);
}
void round_robin(int time_slice){
    scheduling_algorithm(false, false, false, time_slice);
}
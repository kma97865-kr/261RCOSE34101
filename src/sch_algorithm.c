#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "process.h"
#include "queue.h"
#include "job_queue.h"
#include "running_process.h"
#include "sch_algorithm.h"
#include "wait_queue.h"
#define TIME_TICK (1 * BASE_TICK) //UNIX scheduling, recalculate priority every TIME_TICK
#define MAX_IO_TIME (5 * BASE_TICK) //IO burst time
#define PROBABILITY (MAX_TIME) // IO burst probability, 1/50
//static 전역 변수 : 이 소스코드 외에서 사용하지 않음
static bool IS_UNIX = false;
static bool IS_IO = true;
int BASE_PRIORITY = 0;


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
static void setting_priority(Queue queue, int priority_change){ //queue 순회하면서 priority를 갱신
    Node temp_node = queue_front_node(queue);
    Process temp_p;
    while(temp_node != NULL){
        temp_p = queue_node_get_data(temp_node);
        
        int new_priority;
        if(IS_UNIX) new_priority = process_priority(temp_p) + priority_change;
        else new_priority = process_cpu_burst_time(temp_p); //sjf

        //at unix scheduling, assume that priority >= base_priority
        if(IS_UNIX && new_priority < BASE_PRIORITY) new_priority = BASE_PRIORITY;

        process_set_priority(temp_p, new_priority);
        temp_node = queue_node_next_node(temp_node);
    }
}
static bool is_io_occured(){ //let io occur randomly, if occured, yes
    if(rand()%PROBABILITY == 0) return true;
    else return false;
}
static void clear_io_time(Process p){ //clear if process is terminated without io interrupt
    process_set_io_start_time(p, 0);
    process_set_io_end_time(p, 0);
}

void flip_io_interrupt(){
    if(IS_IO) IS_IO = false;
    else IS_IO = true;
}

bool show_io_interrupt(){
    return IS_IO;
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

void scheduling_algorithm(bool priority, bool preemption, bool sjf, int time_slice){
    job_queue_sort();
    
    //handling rule
    if(sjf == true) priority = true;
    if(time_slice < 0){
        //time_slice = 0 -> disable RR, >0 -> enable RR
        printf("Time slice initalizion error\n");
        exit(EXIT_FAILURE);
    }
    
    int time = -1;
    int consumed_time = 0; //RR

    bool is_pr;
    bool is_rr;
    bool is_unix = IS_UNIX;

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
        Process temp_from_wait = wait_queue_to_ready(time);
        while(temp_from_wait != NULL){
            if(priority) priority_queue_push(ready_queue, temp_from_wait); //use priority queue
            else queue_push(ready_queue, temp_from_wait); //use ordinary queue (FCFS)
            temp_from_wait = wait_queue_to_ready(time);
        }

        if(is_unix){//at the Unix scheduling, recalculate priority every TIME_TICK
            if(time%TIME_TICK == 0){
                //setting_priority(job_queue, -1 * TIME_TICK);
                setting_priority(ready_queue, -1 * TIME_TICK);
                setting_priority(wait_queue, -1 * TIME_TICK);
                //increase CPU_usage of running_process
                if(running_process != NULL) process_set_priority(running_process, process_priority(running_process) + TIME_TICK);
            }
        }
        
        //check running process
        //assumption : inital burst time of every process > 0
        if(running_process != NULL){//running_process exits
            int remain_time = running_process_time_consume(1);

            //sjf has to refresh running_process priority for every time
            if(sjf) process_set_priority(running_process, process_cpu_burst_time(running_process));

            if(remain_time > 0){ //still running
                consumed_time++;
                //I/O interrupt code
                    if(IS_IO && is_io_occured()){//Interrupt occured, set io start time and io end time
                        process_set_io_start_time(running_process, (unsigned int) time);
                        process_set_io_end_time(running_process, time + rand()%MAX_IO_TIME + 1);

                        //creat new process to put in terminate_queue and put it
                        Process new_ter_io = process_create();
                        process_copy(running_process, new_ter_io);
                        process_set_end_time(new_ter_io, (unsigned int)time);
                        queue_push(terminate_queue, new_ter_io);
                        
                        //move running process to wait queue
                        wait_queue_push(wait_queue, running_process);
                        
                        //clear running_process, fill from ready_queue 
                        running_process = NULL;
                        check_ready_put_running(time, &consumed_time);

                        //if interrupt and replace are occured, nothing to do more in this time
                        continue;      
                    }
                    //Interrupt not occured -> do nothing here, continue below code
                    
                is_rr = time_slice && (consumed_time >= time_slice); //round_robin check
                is_pr = preemption && check_priority_preemption(); //preemptive check
                if(is_rr || is_pr){ //preemptive code
                    //preemption true -> priority (might) true

                    //termination에 넣을 프로세스 생성 후 push
                    Process new_ter = process_create();
                    process_copy(running_process, new_ter);
                    process_set_end_time(new_ter, (unsigned int)time);
                    clear_io_time(new_ter);
                    queue_push(terminate_queue, new_ter);
                    
                    //현재 running process를 ready_queue에 돌려놓음
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
                clear_io_time(running_process);
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
    setting_priority(job_queue, 0);
    scheduling_algorithm(true, false, true, 0);
}

void preemptive_priority(){
    scheduling_algorithm(true, true, false, 0);
}

void preemptive_sjf(){
    setting_priority(job_queue, 0);
    scheduling_algorithm(true, true, true, 0);
}

void round_robin(int time_slice){
    scheduling_algorithm(false, false, false, time_slice);
}

void custom_scheduling(bool priority, bool preemption, bool sjf, int time_slice){
    if(sjf) priority = true;
    if(time_slice < 0){
        printf("TIME SLICE ERROR\n");
        return;
    }
    scheduling_algorithm(priority, preemption, sjf, time_slice);
}

void unix_scheduling(int base_priority, int time_slice){
    IS_UNIX = true;
    //job queue에 있는 priority에 base 만큼 더해줌
    BASE_PRIORITY = base_priority;
    setting_priority(job_queue, base_priority);
    scheduling_algorithm(true, true, false, time_slice);
    BASE_PRIORITY = 0;
    IS_UNIX = false;
}
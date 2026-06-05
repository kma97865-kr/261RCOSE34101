#include <stdio.h>
#include <stdlib.h>
#include <time.h> //for rand() function(0~INT_MAX)
#include <limits.h> //for INT_MAX
#include "process.h"

//caution!! : MAX_TIME should not be too big, compared to INT_MAX

int p_index = 0;

struct _process{
    int process_index;
    int process_id;
    unsigned int arrival_time;//job queue
    unsigned int cpu_burst_time;
    unsigned int io_start_time;//io interrupt
    unsigned int io_end_time;
    int priority;

    unsigned int start_time;//time on running
    unsigned int end_time;//(might be partial)
};

Process process_create(){
    Process p = malloc(sizeof(struct _process));
    if(p == NULL){
        printf("Process create failure");
        exit(EXIT_FAILURE);
    }
    //initialize process
    p->process_index = p_index++;
    p->process_id = rand(); //0~INT_MAX(LINUX)
    p->arrival_time = rand()%MAX_TIME; //0~MAX_TIME-1
    //p->arrival_time = 0;
    p->cpu_burst_time = rand()%MAX_TIME + 1; //1~MAX_TIME
    p->io_start_time = 0;
    p->io_end_time = 0;
    p->start_time = 0;
    p->end_time = 0;
    p->priority = rand()%MAX_TIME; //프로세스 개수 만큼은 있어야 다양성 보장
    
    return p;
}
void process_destroy(Process p){
    free(p);
    p = NULL;
}
void process_show(Process p){
    if(p == NULL){
        return;
    }
    printf("process_index is %d\n", p->process_index);
    printf("process_id is %d\n", p->process_id);
    printf("arrival_time is %u\n", p->arrival_time);
    printf("cpu burst time is %u\n", p->cpu_burst_time);
    //printf("io_start_time is %u\n", p->io_start_time);
    //printf("io_end time is %u\n", p->io_end_time);
    printf("priority is %d\n", p->priority);
    printf("start time is %u\n", p->start_time);
    printf("end time is %u\n", p->end_time);
    printf("\n");
}

//방어적 프로그램 -> p==NULL 일 경우 수정해야 함
int process_id(Process p){
    return p->process_id;
}
int process_index(Process p){
    return p->process_index;
}

unsigned int process_arrival_time(Process p){
    return p->arrival_time;
}
unsigned int process_cpu_burst_time(Process p){
    return p->cpu_burst_time;
}
unsigned int process_io_start_time(Process p){
    return p->io_start_time;
}
unsigned int process_io_end_time(Process p){
    return p->io_end_time;
}
int process_priority(Process p){
    return p->priority;
}
unsigned int process_start_time(Process p){
    return p->start_time;
}
unsigned int process_end_time(Process p){
    return p->end_time;
}

int process_cpu_time_consume(Process p, int i){
    if(p->cpu_burst_time < (unsigned int) i) {
        printf("process_cpu_time_consume Error\n");
        exit(EXIT_FAILURE);
    }
    else{
        p->cpu_burst_time = p->cpu_burst_time - (unsigned int) i;
        return (int) p->cpu_burst_time;
    }
}

void process_set_arrival_time(Process p, unsigned int u){
    if(p == NULL) return;
    p->arrival_time = u;
}
void process_set_cpu_burst_time(Process p, unsigned int u){
    if(p == NULL) return;
    p->cpu_burst_time = u;
}
void process_set_priority(Process p, int i){
    if(p == NULL) return;
    p->priority = i;
}

void process_set_start_time(Process p, unsigned int u){
    if(p == NULL) return;
    p->start_time = u;
}
void process_set_end_time(Process p, unsigned int u){
    if(p == NULL) return;
    p->end_time = u;
}
void process_set_io_start_time(Process p, unsigned int u){
    if(p == NULL) return;
    p->io_start_time = u;
}
void process_set_io_end_time(Process p, unsigned int u){
    if(p == NULL) return;
    p->io_end_time = u;
}
void process_copy(Process source, Process target){
    *target = *source;
}
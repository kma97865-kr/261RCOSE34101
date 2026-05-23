#include <stdio.h>
#include <stdlib.h>
#include <time.h> //srand(), rand() function
#include "process.h" 
#include "queue.h" 
#include "job_queue.h" //control job_queue
#include "wait_queue.h" //control wait_queue, I/O interrupt
#include "running_process.h" //control running_process
#include "sch_algorithm.h" //scheduling algorithm
#include "config.h" //Initialization, make clean
#include "evaluate.h" //evaluation, Gantt chart



//initialize
//used as external variable at other source file
Queue job_queue = NULL;
Queue ready_queue = NULL;
Queue wait_queue = NULL;
Queue terminate_queue = NULL;
Process running_process = NULL;


int main(void){
    srand((unsigned int)time(NULL));

    config_initialize();
    config_create_process(4);
    //config_create_test_process();
    queue_show(job_queue);;

    //sch algorithm
    preemptive_sjf();

    //queue_show(terminate_queue);

    evaluate_gantt_chart(terminate_queue);

    //terminate_queue의 사이즈만큼 배열을 줘야 함
    int size = queue_size(terminate_queue);
    int *arr = malloc(sizeof(int) * size);
    printf("average waiting time is %d\n", evaluate_waiting_time(terminate_queue, arr));
    printf("average turnaround time is %d\n", evaluate_turnaround_time(terminate_queue, arr));
    free(arr);

    config_clean();
    return 0;
}
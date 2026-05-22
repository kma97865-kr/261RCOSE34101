#include <stdio.h>
#include <stdlib.h>
#include <time.h> //srand(), rand() function
#include "process.h"
#include "queue.h"
#include "config.h"
#include "job_queue.h"
#include "running_process.h"
#include "sch_algorithm.h"
#include "evaluate.h"

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
    config_create_test_process();
    queue_show(job_queue);
    job_queue_sort();
    preemptive_priority();
    //queue_show(terminate_queue);

    evaluate_gantt_chart(terminate_queue);

    //terminate_queue의 사이즈만큼 배열을 줘야 함
    int size = queue_size(terminate_queue);
    int *arr = malloc(sizeof(int) * size);
    printf("waiting time is %d\n", evaluate_waiting_time(terminate_queue, arr));
    printf("turnaround time is %d\n", evaluate_turnaround_time(terminate_queue, arr));
    free(arr);

    config_clean();
    return 0;
}
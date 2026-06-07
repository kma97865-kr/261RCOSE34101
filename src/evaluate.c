#include <stdio.h>
#include <stdlib.h>
#include "evaluate.h"

//waiting time -> sigma(시작 시간) - sigma(종료 시간) + 마지막 종료 시간 - 도착 시간
int evaluate_waiting_time(Queue q, int *arr){
    //int *arr is storage of waiting time.
    int size = queue_size(source_job_queue);

    int *start_sigma_arr = calloc(size, sizeof(int));
    int *end_sigma_arr = calloc(size, sizeof(int));
    int *end_last_arr = calloc(size, sizeof(int));
    int *arrival_time = calloc(size, sizeof(int));

    Node temp = queue_front_node(q);
    while(temp != NULL){
        Process temp_p = queue_node_get_data(temp);
        int index = process_index(temp_p);

        start_sigma_arr[index] += (int)process_start_time(temp_p);
        end_sigma_arr[index] += (int)process_end_time(temp_p);
        end_last_arr[index] = (int) process_end_time(temp_p);
        arrival_time[index] = (int) process_arrival_time(temp_p);

        //around the queue
        temp = queue_node_next_node(temp);
    }


    int sum = 0;
    //size 가 아니라 프로세스 개수(index) 만큼만 돌면 되는데? -> 사용자의 입력으로 매번 다르다
    //Then how? source_job_queue의 size를 재면 된다
    for(int i = 0; i<size; i++){
        arr[i] = start_sigma_arr[i] - end_sigma_arr[i] + end_last_arr[i] - arrival_time[i];
        if(arr[i] < 0){
            printf("evalute_waiting_time failure!");
            exit(EXIT_FAILURE);
        }
        sum += arr[i];
    }

    free(start_sigma_arr);
    free(end_sigma_arr);
    free(end_last_arr);
    free(arrival_time);

    return sum / size;
}

//turnaround_time = end_time - arrival_time
int evaluate_turnaround_time(Queue q, int *arr){
    //arr는 caller에서 이미 할당이 된 배열
    int size = queue_size(source_job_queue);
    int *arrival_arr = calloc(size, sizeof(int));

    //if queue is NULL, return NULL
    Node temp = queue_front_node(q);
    while(temp != NULL){
        Process temp_p = queue_node_get_data(temp);
        arr[process_index(temp_p)] = (int)process_end_time(temp_p);

        if(arrival_arr[process_index(temp_p)] == 0){ //terminate queue has mutiple copy of same process
            //fill arrival time of each process
            arrival_arr[process_index(temp_p)] = (int) process_arrival_time(temp_p);
        }

        //around the queue
        temp = queue_node_next_node(temp);
    }

    int sum = 0;
    for(int i = 0; i<size; i++){
        arr[i] = arr[i] - arrival_arr[i];
        if(arr[i] < 0){
            printf("evalute_turnaround_time failure!");
            exit(EXIT_FAILURE);
        }
        sum += arr[i]; //overflow?
    }
    free(arrival_arr);

    return sum / size; //float?
}

void evaluate_gantt_chart(Queue q) {
    int size = queue_size(q);
    if (size == 0){
        printf("Nothing to print!\n");
        return;
    }

    //printf("\n[ CPU Scheduling Timeline ]\n");
    printf("==================================================================================================\n");
    printf("    TIME    |  PROCESS  |  Arrival time  |  Priority  | Remaining time |  I/O start  |  I/O end  |\n");
    printf("--------------------------------------------------------------------------------------------------\n");

    Node curr_node = queue_front_node(q);
    int current_time = 0;

    for (int i = 0; i < size; i++) {
        Process p = queue_node_get_data(curr_node);

        // IDLE 발생 시
        if (current_time < (int)process_start_time(p)) {
            printf(" %3d ~ %-3d  |   IDLE \n", current_time, process_start_time(p));
            current_time = process_start_time(p);
        }

        // 프로세스 실행 구간
        printf(" %3d ~ %-3d  |  [ P%-2d ]  |       %-2d       |     %-2d     |       %-2d       |      %-3d    |     %-3d   |\n", 
            process_start_time(p), process_end_time(p), process_index(p), process_arrival_time(p), 
            process_priority(p), process_cpu_burst_time(p), process_io_start_time(p), process_io_end_time(p));
        current_time = process_end_time(p);
        curr_node = queue_node_next_node(curr_node);
    }
    printf("==================================================================================================\n");
}

void evaluate_algorithm(){
    //print result of scheduling
    evaluate_gantt_chart(terminate_queue);
    //arr[process->index] 정보가 저장되어 있음 : 추후 활용 가능
    int *arr = malloc(sizeof(int) * queue_size(source_job_queue));
    printf("Average waiting time is %d\n", evaluate_waiting_time(terminate_queue, arr));
    printf("Average turnaround time is %d\n", evaluate_turnaround_time(terminate_queue, arr));

    free(arr);
}
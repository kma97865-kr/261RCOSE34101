#include <stdio.h>
#include <stdlib.h>
#include "evaluate.h"

//waiting time -> sigma(시작 시간) - sigma(종료 시간) + 마지막 종료 시간 - 도착 시간
int evaluate_waiting_time(Queue q, int *arr){
    //int *arr is storage of waiting time.
    int size = queue_size(q);

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

int evaluate_turnaround_time(Queue q, int *arr){
    //arr는 caller에서 이미 할당이 된 배열
    int size = queue_size(q);
    int *arrival_arr = calloc(size, sizeof(int));

    //if queue is NULL, return NULL
    Node temp = queue_front_node(q);
    while(temp != NULL){
        Process temp_p = queue_node_get_data(temp);
        arr[process_index(temp_p)] = (int)process_end_time(temp_p);

        if(arrival_arr[process_index(temp_p)] == 0){
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
    printf("===================================\n");
    printf("    TIME    |  PROCESS  |  Arrival time  |  Priority  | Remaining time\n");
    printf("-----------------------------------\n");

    Node curr_node = queue_front_node(q);
    int current_time = 0;

    for (int i = 0; i < size; i++) {
        Process p = queue_node_get_data(curr_node);

        // IDLE 발생 시
        if (current_time < (int)process_start_time(p)) {
            // %3d를 쓰면 숫자가 3칸을 차지하여 예쁘게 정렬됩니다.
            printf(" %3d ~ %-3d  |  IDLE \n", current_time, process_start_time(p));
            current_time = process_start_time(p);
        }

        // 프로세스 실행 구간
        printf(" %3d ~ %-3d  |  [ P%-2d ]  |  %-2d  |  %-2d  |  %-2d\n", 
            process_start_time(p), process_end_time(p), process_index(p), process_arrival_time(p), 
            process_priority(p), process_cpu_burst_time(p));
        current_time = process_end_time(p);
        curr_node = queue_node_next_node(curr_node);
    }
    printf("=================================\n");
}
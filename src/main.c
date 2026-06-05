#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
Queue source_job_queue = NULL;
Queue job_queue = NULL;
Queue ready_queue = NULL;
Queue wait_queue = NULL;
Queue terminate_queue = NULL;
Process running_process = NULL;

//get one decimal input from user
int get_number_input();

//script_ : print notice, if any value needed, get those
void script_prologue();
void script_algorithm();
void script_custom_sch(int *);
void script_round_robin(int *time);
void script_unix(int *first, int *second);

int main(void){
    srand((unsigned int)time(NULL));

    int input_1, input_2;
    while(1){
        script_prologue();
        input_1 = get_number_input();
        if(input_1 == 0){ //Exit
            config_clean_source();
            return 0;
        }
        else if(input_1 == 1){ //Create new process
            printf("Number of process: ");
            int n_p = get_number_input();
            config_initialize_source(n_p);

            while(1){
                config_initialize(); //get ready to run scheduling algorithm
                script_algorithm();
                input_2 = get_number_input();

                if(input_2 == 0){ //escape current proceses
                    config_clean();
                    break;
                }
                switch(input_2){
                    case 1:
                        first_come_first_served();
                        break;
                    case 2:
                        non_preemptive_priority();
                        break;
                    case 3:
                        non_preemptive_sjf();
                        break;
                    case 4:
                        preemptive_priority();
                        break;
                    case 5:
                        preemptive_sjf();
                        break;
                    case 6:
                        int time_slice;
                        script_round_robin(&time_slice);
                        round_robin(time_slice);
                        break;
                    case 7:
                        int base_priority, time_slice_;
                        script_unix(&base_priority, &time_slice_);
                        unix_scheduling(base_priority, time_slice_);
                        break;
                    case 8:
                        int custom[4];
                        script_custom_sch(custom);
                        custom_scheduling(custom[0], custom[1], custom[2], custom[3]);
                        break;
                    case 9:
                        flip_io_interrupt();
                        continue;
                    case 10:
                        queue_show(source_job_queue);
                        continue;
                    }
                
                evaluate_algorithm();

                config_clean();
            }
        }
    }

    return 0;
}

int get_number_input(){
    char buffer[100];
    int num;
    char extra;
    while(1){
        if(fgets(buffer, sizeof(buffer), stdin) == NULL){
            printf("INPUT ERROR occured\n");
            continue;
        }
        buffer[strcspn(buffer, "\n")] = 0;

        if(strlen(buffer) == 0) continue;

        int parsed_count = sscanf(buffer, "%d%c", &num, &extra);
        if(parsed_count == 1) return num;
        else{
            printf("Invalid Input. Only one decimal is permitted\n");
            continue;
        }
    }
}

void script_prologue(){
    printf("\nHello, it's Yeonghun's CPU schduling simulator\n");
    printf("1. Create New processes\n");
    printf("0. Exit\n");
    printf("Choice: ");
}
void script_algorithm(){
    printf("\n1. First Come First Served\n");
    printf("2. Non Preemptive Priority\n");
    printf("3. Non Preemptive SJF\n");
    printf("4. Preemptive Priority\n");
    printf("5. Preemptive SJF\n");
    printf("6. Round Robin\n");
    printf("7. Unix_Scheduling(Dynamic Priority)\n");
    printf("8. Your Custom Scheduling\n");
    printf("\n9. Flip io interrupt. Current status : %s\n", show_io_interrupt() ? "true" : "false");
    printf("10. Show current processes\n");
    printf("0. Back to process create step\n");
    printf("Choice: ");
}
void script_custom_sch(int *custom){
    printf("\nWe need 4 parameter: priority, preemption, sjf, time_slice\n");
    printf("Give 0 or 1 regarding each parameter except time_slice\n");
    printf("Give natural number for time_slice used for RR\n");
    for(int i = 0; i<4; i++){
        printf("Input for %dth parameter: ", i);
        custom[i] = get_number_input();
    }
}
void script_round_robin(int *time){
    printf("\nCurrent max burst time is %d\n", MAX_TIME);
    printf("Be cautious that too long or short time_slice makes algorithm uneffective\n");
    printf("Give natural number for time_slice used for RR: ");
    while((*time = get_number_input()) <= 0){
        printf("Invalid input. Input for ime_slice: ");
    }
}
void script_unix(int *first, int *second){
    printf("\nWe need 2 parameters: base_priority, time_slice\n");
    printf("Give number equal or greater than 0 for base_priority\n");
    printf("Give natural number for time_slice\n");
    printf("Input for base priority: ");
    while((*first = get_number_input()) < 0){
        printf("Invalid input. Input for base priority: ");
    }
    printf("Current max burst time is %d\n", MAX_TIME);
    printf("Be cautious that too long or short time_slice makes algorithm uneffective\n");
    printf("Input for time_slice: ");
    while((*second = get_number_input()) <= 0){
        printf("Invalid input. Input for time_slice: ");
    }
}
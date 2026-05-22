#include <stdio.h>
#include <stdlib.h>
#include "running_process.h"

int running_process_time_consume(int i){
    if(running_process == NULL){
        printf("running_process_time_consume(int) error\n");
        exit(EXIT_FAILURE);
    }
    int u = process_cpu_time_consume(running_process, i);
    return u;
    //-1 : fail to consume
    //0 : no reamining burst time
    //>1 : still running   
}

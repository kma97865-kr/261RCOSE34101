#include <stdio.h>
#include <stdlib.h>
#include <time.h> //srand(), rand() function
#include "process.h"
#include "queue.h"
#include "config.h"
#include "job_queue.h"
#include "running_process.h"

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
    config_create_process(10);
    //queue_show(job_queue);
    job_queue_sort();
    config_clean();
    

    return 0;
}
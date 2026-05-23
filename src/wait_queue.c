#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "queue.h"
#include "wait_queue.h"

Process wait_queue_to_ready(int time){
    if(queue_is_empty(wait_queue)) return NULL;

    Process temp = queue_front_process(wait_queue);
    if(process_io_end_time(temp) <= (unsigned int)time){
        return queue_pop(wait_queue);
    }else{
        return NULL;
    }
}
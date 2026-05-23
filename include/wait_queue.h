#ifndef WAIT_QUEUE_H
#define WAIT_QUEUE_H

#include "queue.h"
#include "process.h"

extern Queue wait_queue;
extern Queue ready_queue;

//wait queue -> ready_queue
Process wait_queue_to_ready(int time);

//wait queue push is in queue.h

#endif
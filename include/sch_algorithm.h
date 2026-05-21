#ifndef SCH_ALGORITHM_H
#define SCH_ALGORITHM_H
#include "process.h"
#include "queue.h"

extern Queue job_queue;
extern Queue ready_queue;
extern Queue wait_queue;
extern Queue terminate_queue;
extern Process running_process;

void first_come_first_served();

void check_ready_move_running(int time);

#endif
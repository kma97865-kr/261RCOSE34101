#ifndef SCH_ALGORITHM_H
#define SCH_ALGORITHM_H
#include "process.h"
#include "queue.h"

extern Queue job_queue;
extern Queue ready_queue;
extern Queue wait_queue;
extern Queue terminate_queue;
extern Process running_process;


void scheduling_algorithm(bool priority, bool preemption, bool sjf);

void first_come_first_served();

void non_preemptive_priority();
void non_preemptive_sjf();

void preemptive_priority();
void preemptive_sjf();


#endif
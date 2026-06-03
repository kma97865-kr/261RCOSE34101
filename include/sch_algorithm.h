#ifndef SCH_ALGORITHM_H
#define SCH_ALGORITHM_H
#include "process.h"
#include "queue.h"

extern Queue job_queue;
extern Queue ready_queue;
extern Queue wait_queue;
extern Queue terminate_queue;
extern Process running_process;


void scheduling_algorithm(bool priority, bool preemption, bool sjf, int time_slice);
//priority 
//preemption : true->preemptive, false->non_preemptive
//sjf : true -> sjf, false -> no sjf(do not use burst time as priority)
//time_slice : 0 -> disable round robin, greater than 0 -> enable round robin

void first_come_first_served();

void non_preemptive_priority();
void non_preemptive_sjf();

void preemptive_priority();
void preemptive_sjf();

void round_robin(int time_slice);

void custom_scheudling(bool priority, bool preemption, bool sjf, int time_slice);

void unix_scheduling(int base_priority, int time_slice);
#endif
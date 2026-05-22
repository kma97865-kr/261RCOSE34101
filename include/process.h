#ifndef PROCESS_H
#define PROCESS_H

typedef struct _process *Process; //Process is pointer of strcut _process

//initialize
Process process_create(void);
void process_destroy(Process p);

//debug
void process_show(Process p);

//Getter
int process_id(Process p);
int process_index(Process p);
unsigned int process_arrival_time(Process p);
unsigned int process_cpu_burst_time(Process p);
unsigned int process_io_start_time(Process p);
unsigned int process_io_end_time(Process p);
int process_priority(Process p);
unsigned int process_start_time(Process p);
unsigned int process_end_time(Process p);

//Setter
int process_cpu_time_consume(Process p, int i);
//decrease cpu_burst_time for time i;
//if fail, return -1. else, return remaining burst time

void process_set_arrival_time(Process p, unsigned int u);
void process_set_cpu_burst_time(Process p, unsigned int u);
void process_set_priority(Process p, int i);

void process_set_start_time(Process p, unsigned int u);
void process_set_end_time(Process p, unsigned int u);
void process_copy(Process source, Process target);

#endif
#ifndef PROCESS_H
#define PROCESS_H

typedef struct _process *Process; //Process is pointer of strcut _process


Process process_create(void);
void process_destroy(Process p);

void process_show(Process p);

int process_id(Process p);
unsigned int process_arrival_time(Process p);
unsigned int process_cpu_burst_time(Process p);
unsigned int process_io_start_time(Process p);
unsigned int process_io_end_time(Process p);
int process_priority(Process p);
unsigned int process_start_time(Process p);
unsigned int process_end_time(Process p);

#endif
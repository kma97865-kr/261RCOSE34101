#ifndef CONFIG_H
#define CONFIG_H

#include "process.h"
#include "queue.h"

extern Queue job_queue;
extern Queue ready_queue;
extern Queue wait_queue;
extern Queue terminate_queue;
extern Process runnning_process;

void config_initialize(void); //create extern values
void config_create_process(int n); //create n process and push into job queue

void config_clean(); //deallocate all -> destroy extern values

#endif
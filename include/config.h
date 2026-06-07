#ifndef CONFIG_H
#define CONFIG_H

#include "process.h"
#include "queue.h"

extern Queue source_job_queue;
extern Queue job_queue;
extern Queue ready_queue;
extern Queue wait_queue;
extern Queue terminate_queue;
extern Process runnning_process;

void config_initialize_source(int n); //create source_job_queue
void config_initialize(void); //create extern values
void config_create_test_process(); //for debug

void config_clean(); //deallocate all -> destroy extern values
void config_clean_source();

#endif
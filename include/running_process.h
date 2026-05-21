#ifndef RUNNING_PROCESS_H
#define RUNNING_PROCESS_H
#include "process.h"

extern Process running_process;

//-1 : fail to consume
//0 : no reamining burst time
//>1 : still running   //if no valid running_process: exit
int running_process_time_consume(int i);

#endif
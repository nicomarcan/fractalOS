#ifndef SCHEDULER_H
#define SCHEDULER_H
#include <stdint.h>


void insertProcess(void * entry_point); 

/*
 * Receives the previous process' stack
 * pointer and returns the stack pointer
 * to the process to execute.
 */
void * switchStackPointer(void * rsp);


#endif

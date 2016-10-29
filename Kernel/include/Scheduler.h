#ifndef SCHEDULER_H
#define SCHEDULER_H
#include <stdint.h>


void insertProcess(void * entry_point,uint64_t rax); 
/*
 * Receives the previous process' stack
 * pointer and returns the stack pointer
 * to the process to execute.
 */
void * switchStackPointer(void * rsp);

/*
 * Called only once after initializing
 * the kernel. Clears interrupts and calls
 * first process.
 */
void begin();

/*
 * Creates a new process using
 * the parameter as the entry point
 */
void fkexec(void * entry_point);

/*
 * Removes the current process from the
 * process table
 */
void exit();
#endif

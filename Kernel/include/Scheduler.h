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
 * Forks the current process.
 * rax is set to 1 in the invoking process,
 * and to 0 in the newly created process.
 */
void fork();

/*
 * Changes the instruction pointer of
 * the current process to the passed value.
 */
void exec(void * rip);

#endif

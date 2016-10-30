#ifndef SCHEDULER_H
#define SCHEDULER_H
#include <stdint.h>


struct Args{
	uint64_t argc;
	uint8_t ** argv;
};

struct ProcessInfo{
	uint64_t process_count;
	uint64_t * PIDs;
	uint8_t ** descrs;
};
typedef struct ProcessInfo ProcessInfo;
typedef struct Args Args;
/*
 * Creates a process and inserts it into the
 * process' list
 */
void insertProcess(void * entry_point,uint64_t rax,uint64_t rdi, uint64_t rsi,uint8_t * descr);

/*
 * Deletes a process from the scheduler's internal
 * list. If the process to delete is the current process,
 * it yields.
 */
void deleteProcessScheduler(uint64_t pid);
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
void fkexec(void * entry_point,uint8_t * descr,Args * args);

/*
 * Switches the context to the next
 * process in the list.
 */
void yield();

/*
 * Removes the current process from the
 * process table and yields.
 */
void exit();

/*
 * Returns a pointer to ProcessInfo
 * containing the amount of processes,
 * their PIDs and stored descriptions.
 */
void * ps();
#endif

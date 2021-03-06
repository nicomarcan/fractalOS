#ifndef SCHEDULER_H
#define SCHEDULER_H
#include <stdint.h>
#include <Mutex.h>

#define SHELLPID 1

struct Args{
	uint64_t argc;
	uint8_t ** argv;
	uint8_t fg;
};

struct ProcessInfo{
	uint64_t process_count;
	uint64_t * PIDs;
	uint8_t ** descrs;
	uint8_t * status;
};
typedef struct ProcessInfo ProcessInfo;
typedef struct Args Args;
/*
 * Creates a process and inserts it into the
 * process' list
 */
uint64_t insertProcess(void * entry_point,uint64_t rax,uint64_t rdi, uint64_t rsi,uint8_t * descr,uint8_t fg);

/*
 * Deletes a process from the scheduler's internal
 * list. If the process to delete is the current process,
 * it yields.
 */
void deleteProcessScheduler(uint64_t pid);

/*
 * Wakes a sleeping or waiting
 * process.
 */
void wake(uint64_t pid);
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
uint64_t fkexec(void * entry_point,uint8_t * descr,Args * args);

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

/*
 * Waits until a wake or kill signal
 * is sent.
 */
 void wait();

 /*
  * Make a process wait.
  */
 void mkwait(uint64_t pid);


  /*
   * Tells whether the current process
   * is the foreground process or not.
   */
 uint8_t isFg();

 /*
  * Gives the foreground to the process
  * with process id pid.
  */
 void giveFg(uint64_t pid);

 /*
  * Returns the pid of the current process.
  */
 uint64_t currPid();

 /*
  * Returns the parent pid of the current
  * process.
  */
 uint64_t currPpid();
	
	
	
 uint64_t disableScheduler();	
	
	
 void enableScheduler(uint64_t v);	
 
 void release_lock_and_sleep(mutex * m);

#endif

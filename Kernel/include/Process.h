#ifndef PROCESS_H
#define PROCESS_H
#include <stdint.h>

struct process {
	void * entry_point;
	void * stack_base;
	void * stack_pointer;
	uint64_t stack_npages;
	uint64_t pid;
};

typedef struct process Process;

Process * newProcess(void * entry_point,uint64_t rax);
void deleteProcess(Process * p);
void * fillStackFrame(void * entry_point,void * stack_base,uint64_t rax);

#endif

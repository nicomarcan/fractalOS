#ifndef PROCESS_H
#define PROCESS_H
#include <stdint.h>

struct process {
	void * entry_point;
	void * stack_base;
	void * stack_pointer;
	uint64_t stack_npages;
};

typedef struct process Process;

Process * newProcess(void * entry_point);
void deleteProcess(Process * p);
void * fillStackFrame(void * entry_point,void * stack_base);

#endif

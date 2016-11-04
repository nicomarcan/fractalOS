#ifndef PROCESS_H
#define PROCESS_H
#include <stdint.h>

struct process {
	void * entry_point;
	void * stack_base;
	void * stack_pointer;
	uint64_t stack_npages;
	uint64_t pid;
	uint64_t ppid;
	uint8_t fg;
};

typedef struct process Process;

Process * newProcess(void * entry_point,uint64_t rax,uint64_t rdi, uint64_t rsi,uint64_t ppid,uint8_t fg);
void deleteProcess(Process * p);
void * fillStackFrame(void * entry_point,void * stack_base,uint64_t rax,uint64_t rdi, uint64_t rsi);

#endif

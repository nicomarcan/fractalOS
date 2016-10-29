#include <Process.h>
#include <liballoc.h>
#define INIT_STACK_PAGES 2

static uint64_t pids = 0;

struct StackFrame {
	//Registers restore context
	uint64_t gs;
	uint64_t fs;
	uint64_t r15;
	uint64_t r14;
	uint64_t r13;
	uint64_t r12;
	uint64_t r11;
	uint64_t r10;
	uint64_t r9;
	uint64_t r8;
	uint64_t rsi;
	uint64_t rdi;
	uint64_t rbp;
	uint64_t rdx;
	uint64_t rcx;
	uint64_t rbx;
	uint64_t rax;

	//iretq hook
	uint64_t rip;
	uint64_t cs;
	uint64_t eflags;
	uint64_t rsp;
	uint64_t ss;
	uint64_t base;
};

typedef struct StackFrame StackFrame;
void * toStackAddress(void * page);

Process * newProcess(void * entry_point,uint64_t rax){
	void * stack_base = liballoc_alloc(INIT_STACK_PAGES);
	Process * ans = (Process *)la_malloc(sizeof(Process));
	ans->entry_point = entry_point;
	ans->stack_base = stack_base;
	ans->stack_npages = INIT_STACK_PAGES;
	ans->stack_pointer = fillStackFrame(entry_point,toStackAddress(stack_base),rax);
	ans->pid = pids++;
	return ans;
}

void deleteProcess(Process * p){
	liballoc_free(p->stack_base,p->stack_npages);
	la_free(p);
	return ;
}

/*
 * Stack grows downwards
 */
void * toStackAddress(void * page){
	return (uint8_t *)page + PSIZE;
}

void * fillStackFrame(void * entry_point, void * stack_base,uint64_t rax) {
	StackFrame * frame = (StackFrame*)(stack_base - sizeof(StackFrame) -1);
	frame->gs =		0x001;
	frame->fs =		0x002;
	frame->r15 =	0x003;
	frame->r14 =	0x004;
	frame->r13 =	0x005;
	frame->r12 =	0x006;
	frame->r11 =	0x007;
	frame->r10 =	0x008;
	frame->r9 =		0x009;
	frame->r8 =		0x00A;
	frame->rsi =	0x00B;
	frame->rdi =	0x00C;
	frame->rbp =	0x00D;
	frame->rdx =	0x00E;
	frame->rcx =	0x00F;
	frame->rbx =	0x010;
	frame->rax =	rax;
	
	frame->rip =	(uint64_t)entry_point;
	frame->cs =		0x008;
	frame->eflags = 0x202;
	frame->rsp =	(uint64_t)&(frame->base);
	frame->ss = 	0x000;
	frame->base =	0x000;
	return frame;
}

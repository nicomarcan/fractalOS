#include <Scheduler.h>
#include <Process.h>
#include <liballoc.h>
#include <asmlib.h>
#define NULL 0

struct ProcessNode {
	Process * p;
	uint8_t * descr;
	struct ProcessNode * next;
	struct ProcessNode * prev;
};

typedef struct ProcessNode ProcessNode;
extern changeContextFromRsp(uint64_t rsp);
void schedule();

ProcessNode * current = NULL;
static int process_count = 0;

void insertProcess(void * entry_point,uint64_t rax,uint64_t rdi, uint64_t rsi,uint8_t * descr){
	Process * p = newProcess(entry_point,rax,rdi,rsi);
	ProcessNode * pnode = la_malloc(sizeof(ProcessNode));
	pnode->p = p;
	pnode->descr = descr;
	if(current == NULL){
		current = pnode;
		pnode->next = pnode;
		pnode->prev = pnode;
	} else {
		ProcessNode * tmp = current->next;
		current->next = pnode;
		pnode->next = tmp;
		pnode->prev = current;
		tmp->prev  = pnode;
	}
	process_count++;
	return;
}

void deleteProcessScheduler(uint64_t pid){
	if(current->p->pid == pid){
		exit();
	} else {
		ProcessNode * curr = current;
		for(int i=0; i<process_count ; i++ , curr = curr->next){
			if(curr->p->pid == pid){
				ProcessNode * cn = curr;
				ProcessNode * prev = cn->prev;
				ProcessNode * next = cn->next;
				prev->next = next;
				next->prev = prev;
				deleteProcess(cn->p);
				la_free(cn);
				process_count--;
			}
		}
	}
}

void schedule(){
	current = current->next;
	return;
}

void * switchStackPointer(void * rsp){
	current->p->stack_pointer = rsp;
	schedule();
	return current->p->stack_pointer;
}

void fkexec(void * entry_point,uint8_t * descr,Args * args){
	insertProcess(entry_point,0,args->argc,args->argv,descr);
}

void begin(){
	_sti();
	((void (*)(void))(current->p->entry_point))();
}

void yield(){
	schedule();
	changeContextFromRsp(current->p->stack_pointer);
}

void exit(){
	ProcessNode * cn = current;
	ProcessNode * prev = cn->prev;
	ProcessNode * next = cn->next;
	prev->next = next;
	next->prev = prev;
	deleteProcess(cn->p);
	la_free(cn);
	process_count--;
	yield();
}

void * ps(){
	ProcessInfo * ans = la_malloc(sizeof(ProcessInfo));
	ans->process_count = process_count;
	ans->PIDs = la_malloc(sizeof(uint64_t)*process_count);
	ans->descrs = la_malloc(sizeof(uint8_t *)*process_count);
	ProcessNode * curr = current;
	for(int i=0; i<process_count ; i++ , curr = curr->next){
		(ans->PIDs)[i]=curr->p->pid;
		(ans->descrs)[i]=curr->descr;
	}
	return (void *)ans;
}

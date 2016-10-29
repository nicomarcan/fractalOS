#include <Scheduler.h>
#include <Process.h>
#include <liballoc.h>
#include <asmlib.h>
#define NULL 0

struct ProcessNode {
	Process * p;
	struct ProcessNode * next;
	struct ProcessNode * prev;
};

typedef struct ProcessNode ProcessNode;
extern changeContextFromRsp(uint64_t rsp);
void schedule();

ProcessNode * current = NULL;

void insertProcess(void * entry_point,uint64_t rax){
	Process * p = newProcess(entry_point,rax);
	ProcessNode * pnode = la_malloc(sizeof(ProcessNode));
	pnode->p = p;
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
	return;
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

void fkexec(void * entry_point){
	insertProcess(entry_point,0);
}

void begin(){
	_sti();
	((void (*)(void))(current->p->entry_point))();
}

void exit(){
	ProcessNode * cn = current;
	ProcessNode * prev = cn->prev;
	ProcessNode * next = cn->next;
	prev->next = next;
	next->prev = prev;
	deleteProcess(cn->p);
	la_free(cn);
	schedule();
	changeContextFromRsp(current->p->stack_pointer);
}

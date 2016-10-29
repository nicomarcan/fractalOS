#include <Scheduler.h>
#include <Process.h>
#define NULL 0

struct ProcessNode {
	Process * p;
	struct ProcessNode * next;
};

typedef struct ProcessNode ProcessNode;
void schedule();

ProcessNode * current = NULL;

void insertProcess(void * entry_point){
	Process * p = newProcess(entry_point);
	ProcessNode * pnode = la_malloc(sizeof(ProcessNode));
	pnode->p = p;
	if(current == NULL){
		current = pnode;
		pnode->next = pnode;
	} else {
		ProcessNode * tmp = current->next;
		current->next = pnode;
		pnode->next = tmp;
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

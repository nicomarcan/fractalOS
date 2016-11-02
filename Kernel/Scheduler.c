#include <Scheduler.h>
#include <Process.h>
#include <liballoc.h>
#include <asmlib.h>
#define NULL 0

typedef struct ProcessNode {
	Process * p;
	uint8_t * descr;
	struct ProcessNode * next;
	struct ProcessNode * prev;
} ProcessNode;

typedef struct SleepNode {
	uint64_t ticks;
	ProcessNode * pn;
	struct SleepNode * next;
} SleepNode;

extern changeContextFromRsp(uint64_t rsp);
void schedule();
void sleep(uint64_t ticks);
static ProcessNode * deleteProcessNode(ProcessNode * p);
static SleepNode * decrementTicksR(SleepNode * sn);

ProcessNode * current = NULL;
static int process_count = 0;

SleepNode * sleeping = NULL;

void insertProcess(void * entry_point,uint64_t rax,uint64_t rdi, uint64_t rsi,uint8_t * descr){
	Process * p;
	if(current == NULL){
		p=newProcess(entry_point,rax,rdi,rsi,0);
	} else {
		p=newProcess(entry_point,rax,rdi,rsi,current->p->ppid);
	}
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
	if(pid == 0 || pid == 1){
		return;
	}
	if(current->p->pid == pid){
		exit();
	} else {
		ProcessNode * curr = current;
		for(int i=0; i<process_count ; i++ , curr = curr->next){
			if(curr->p->pid == pid){
				ProcessNode * cn = curr;
				deleteProcessNode(cn);
				deleteProcess(cn->p);
				la_free(cn);
				process_count--;
				break;
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

void exit(){
	ProcessNode * cn = current;
	current = deleteProcessNode(cn);
	deleteProcess(cn->p);
	la_free(cn);
	process_count--;
	changeContextFromRsp(current->p->stack_pointer);
}

static ProcessNode * deleteProcessNode(ProcessNode * n){
	ProcessNode * cn = n;
	ProcessNode * prev = cn->prev;
	ProcessNode * next = cn->next;
	prev->next = next;
	next->prev = prev;
	return next;
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


void sys_sleep(uint64_t ticks){
	ProcessNode * cn = current;
	deleteProcessNode(cn);
	process_count--;
	SleepNode * sn = la_malloc(sizeof(SleepNode));
	sn->ticks = ticks;
	sn->pn = cn;
	if (sleeping == NULL){
		sleeping = sn;
		sn->next = NULL;
	} else {
		sn->next = sleeping;
		sleeping = sn;
	}
	yield();
}

void decrementTicks(){
	sleeping = decrementTicksR(sleeping);
}

static SleepNode * decrementTicksR(SleepNode * sn){
	if(sn == NULL){
		return NULL;
	}
	if(sn->ticks == 0){
		SleepNode * t = sn->next;

		ProcessNode * tmp  = current->next;
		current->next = sn->pn;
		sn->pn->prev = current;
		sn->pn->next = tmp;
		tmp->prev = sn->pn;
		process_count ++;
		la_free(sn);
		return decrementTicksR(t);
	}
	sn->ticks --;
	sn->next = decrementTicksR(sn->next);
	return sn;
}

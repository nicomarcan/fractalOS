#include <stdint.h>
#include <cond_variable.h>
#include <stdbool.h>
#include <queue.h>
#include <c_syscall.h>
#include <c_string.h>
#define NULL 0

typedef struct cond_variable {
	uint64_t id;
	uint8_t * name;
	Queue_p q;
} cond_variable;


struct CVNode{
	cond_variable * cv;
	struct CVNode * next;
};

typedef struct CVNode CVNode;

static mutex m;
static uint64_t condvar_number = 0;
static uint64_t historic = 0;
static CVNode * nodes = NULL;

static cond_variable * cvinit(uint8_t * name);
static CVNode * cond_variable_destroy_rec(CVNode * n,cond_variable * cv);


void cvinitlock(){
	mutex_nameinit(&m,"condvar_mainlock");
}

cond_variable * cond_variable_init(){
	mutex_lock(&m);
	cond_variable * cv = cvinit(NULL);
	mutex_unlock(&m);
	return cv;
}

cond_variable * cond_variable_nameinit(uint8_t * name){
	if(cond_variable_getbyname(name)!=NULL){
		return NULL;
	}
	mutex_lock(&m);
	cond_variable * cv = cvinit(name);
	mutex_unlock(&m);
	return cv;
}


static cond_variable * cvinit(uint8_t * name){
    cond_variable * cv = malloc(sizeof(cond_variable));
	cv->q = new_queue();
	cv->id = historic;
	cv->name = name;
	condvar_number ++;
	historic++;
	CVNode * n = malloc(sizeof(CVNode));
	n->cv = cv;
	n->next = nodes;
	nodes = n;
    return cv;
}

cond_variable * cond_variable_getbyname(uint8_t * name){
	if(name == NULL){
		return NULL;
	}
	CVNode * n = nodes;
	for( ; n!=NULL ; n = n->next){
		if(c_strcmp(name,n->cv->name) == 0){
			return n->cv;
		}
	}
	return NULL;
}

void cond_variable_destroy(cond_variable * cv)
{
	mutex_lock(&m);
	nodes = cond_variable_destroy_rec(nodes,cv);
	mutex_unlock(&m);
}

static CVNode * cond_variable_destroy_rec(CVNode * n,cond_variable * cv){
	if(n == NULL){
		return NULL;
	}
	if(n->cv->id == cv->id){
		CVNode * next = n->next;
		q_destroy(cv->q);
		free(cv);
		free(n);
		condvar_number --;
		return next;
	}
	n->next = cond_variable_destroy_rec(n->next,cv);
	return n;
}


void cond_variable_wait(cond_variable * c, mutex * m)
{
	/* add thread to cond_variable queue */
	enque(c->q, getPid());
	/* sleep thread and release lock atomically */

	release_lock_and_sleep(m);
	/* re-aquire lock */
	mutex_lock(m);
}

void cond_variable_signal(cond_variable * c)
{
	if (!q_is_empty(c->q)) {
		kill(deque(c->q), 1);
	}
}


CVInfo * cond_variable_info(){
	CVInfo * ret = malloc(sizeof(CVInfo));
	ret->cvnum = condvar_number;
	ret->cvids = malloc(condvar_number*sizeof(uint64_t));
	ret->descrs = malloc(condvar_number*sizeof(uint8_t*));
	CVNode * n = nodes;
	for(int i=0 ; n!=NULL ; i++ , n = n->next){
		ret->cvids[i] = n->cv->id;
		ret->descrs[i] = n->cv->name;
	}
	return ret;
}

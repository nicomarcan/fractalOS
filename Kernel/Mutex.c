#include <Mutex.h>
#include <asmlib.h>
#include <trylock.h>
#include <Scheduler.h>

#define NULL 0

struct pidnode {
	uint64_t pid;
	struct pidnode * next;
};

typedef struct pidnode pidnode;

struct MutexNode {
	mutex * m;
	pidnode * first;
	pidnode * last;
	uint64_t waitn;
	struct MutexNode * next;
};

typedef struct MutexNode MutexNode;

extern int64_t strcmp(const uint8_t * a, const uint8_t * b) ;
static MutexNode * mutex_destroy_rec(MutexNode * n,mutex * m);
static MutexNode * mutexnode_getbyid(uint64_t id);
static mutex * minit(mutex * m,uint8_t * name);
static uint64_t mutex_count = 0;
static uint64_t historic = 0;
static MutexNode * nodes = NULL;

uint64_t mutex_init(mutex * m){
	return minit(m,NULL)->id;
}

uint64_t mutex_nameinit(mutex * m,uint8_t * name){
	return minit(m,name)->id;
}

static mutex * minit(mutex * m,uint8_t * name){
	disableScheduler();
	
	m->m = 0;
	m->id = historic;
	m->name = name;
	MutexNode * n = la_malloc(sizeof(MutexNode));
	n->m = m;
	n->next = nodes;
	
	n->waitn = 0;
	n->first = n->last = NULL;
	
	nodes = n;
	mutex_count ++;
	historic ++;
	
	enableScheduler();
	return m;
}

mutex * mutex_getbyname(uint8_t * name){
	MutexNode * n = nodes;
	for(; n != NULL ; n = n->next){
		if(strcmp(name,n->m->name)==0){
			return n->m;
		}
	}
	return NULL;
}

static MutexNode * mutexnode_getbyid(uint64_t id){
	MutexNode * n = nodes;
	for(; n != NULL ; n = n->next){
		if(n->m->id == id){
			return n;
		}
	}
	return NULL;
}

void mutex_destroy(mutex * m){
	disableScheduler();
	nodes = mutex_destroy_rec(nodes,m);
	enableScheduler();
}

static MutexNode * mutex_destroy_rec(MutexNode * n,mutex * m){
	if(n == NULL){
		return NULL;
	}
	if(n->m->id == m->id){
		mutex_count --;
		MutexNode * ret = n->next;
		la_free(n);
		return ret;
	}
	n->next = mutex_destroy_rec(n->next,m);
	return n;
}

void mutex_lock(mutex * m){
	if(!itrylock(&(m->m))){
		disableScheduler();
		MutexNode * mn = mutexnode_getbyid(m->id);
		pidnode * pidn = la_malloc(sizeof(pidnode));
		pidn->pid = currPid();
		if(mn->waitn == 0){
			pidn->next = NULL;
			mn->first = mn->last = pidn;
		} else {
			mn->last->next = pidn;
			pidn->next = NULL;
			mn->last = pidn;
		}
		mn->waitn ++;
		enableScheduler();
		wait();
	}
}

void mutex_unlock(mutex * m){
	disableScheduler();
	MutexNode * mn = mutexnode_getbyid(m->id);
	if(mn->waitn > 0){
		pidnode * pidn = mn->first;
		wake(pidn->pid);
		if(mn->waitn == 1){
			mn->first = mn->last = NULL;
		} else {
			mn->first = pidn->next;
		}
		la_free(pidn);
		mn->waitn --;
	} else {
		itryunlock(&(m->m));
	}
	enableScheduler();
	
}

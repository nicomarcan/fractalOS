#include <Mutex.h>
#include <asmlib.h>
#define NULL 0


struct MutexNode {
	mutex * m;
	struct MutexNode * next;
};

typedef struct MutexNode MutexNode;

extern int64_t strcmp(const uint8_t * a, const uint8_t * b) ;
static MutexNode * mutex_destroy_rec(MutexNode * n,mutex * m);
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
	m->m = 0;
	m->id = historic;
	m->name = name;
	MutexNode * n = la_malloc(sizeof(MutexNode));
	n->m = m;
	n->next = nodes;
	nodes = n;
	mutex_count ++;
	historic ++;
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

void mutex_destroy(mutex * m){
	nodes = mutex_destroy_rec(nodes,m);
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
	enter_region(&(m->m));
}

void mutex_unlock(mutex * m){
	leave_region(&(m->m));
}

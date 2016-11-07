#include <Mutex.h>
#include <asmlib.h>

static uint64_t mutex_count = 0;


void mutex_init(mutex * m){
	m->m = 0;
	mutex_count ++;
}

void mutex_init_lock(mutex * m){
	m->m = 1;
	mutex_count++;
}
void mutex_destroy(mutex * m){
	mutex_count --;
}

void mutex_lock(mutex * m){
	enter_region(&(m->m));
}

void mutex_unlock(mutex * m){
	leave_region(&(m->m));
}

#include <Mutex.h>
#include <asmlib.h>


void mutex_lock(uint64_t * mutex){
	enter_region(mutex);
}

void mutex_unlock(uint64_t * mutex){
	leave_region(mutex);
}

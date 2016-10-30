#include <Mutex.h>
#include <asmlib.h>


void mutex_lock(uint8_t * mutex){
	enter_region(mutex);
}

void mutex_unlock(uint8_t * mutex){
	leave_region(mutex);
}

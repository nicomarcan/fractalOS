#ifndef MUTEX_H
#define MUTEX_H
#include <stdint.h>


typedef struct mutex {
	uint64_t m;
} mutex;


/*
 * Initializes the mutex.
 */
void mutex_init(mutex * m);

/*
 * Destroys the mutex.
 */
void mutex_destroy(mutex * m);
/*
 * Atomically locks the mutex.
 * If the mutex is locked, yields.
 */
void mutex_lock(mutex * m);

/*
 * Unlocks the mutex.
 */
void mutex_unlock(mutex * m);


#endif

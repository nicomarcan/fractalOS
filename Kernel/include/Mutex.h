#ifndef MUTEX_H
#define MUTEX_H
#include <stdint.h>

/*
 * Atomically locks the mutex.
 * If the mutex is locked, yields.
 */
void mutex_lock(uint64_t * mutex);

/*
 * Unlocks the mutex.
 */
void mutex_unlock(uint64_t * mutex);


#endif

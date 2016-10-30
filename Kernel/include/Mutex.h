#ifndef MUTEX_H
#define MUTEX_H
#include <stdint.h>

/*
 * Atomically locks the mutex.
 * If the mutex is locked, yields.
 */
void mutex_lock(uint8_t * mutex);

/*
 * Unlocks the mutex.
 */
void mutex_unlock(uint8_t * mutex);


#endif

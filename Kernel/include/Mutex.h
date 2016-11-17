#ifndef MUTEX_H
#define MUTEX_H
#include <stdint.h>


typedef struct mutex {
	uint64_t m;
	uint64_t id;
	uint8_t * name;
} mutex;


/*
 * Initializes the mutex.
 */
uint64_t mutex_init(mutex * m);

/*
 * Initializes the mutex and sets its name.
 */
uint64_t mutex_nameinit(mutex * m,uint8_t * name);

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

/*
 * Obtains a mutex by name.
 */
mutex * mutex_getbyname(uint8_t * name);


#endif

#ifndef TRYLOCK_H
#define TRYLOCK_H

#include <stdbool.h>
#include <stdint.h>

typedef struct TryLock {
    uint64_t lock;
} TryLock;

typedef struct TryLock TryLock;

TryLock * tryinit();
bool trylock(TryLock * tl);
void tryunlock(TryLock * tl);

bool itrylock(uint64_t * l);
void itryunlock(uint64_t * l);

#endif

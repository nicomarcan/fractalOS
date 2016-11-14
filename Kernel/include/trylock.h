#ifndef TRYLOCK_H
#define TRYLOCK_H

#include <stdbool.h>

typedef struct TryLock TryLock;

TryLock * tryinit();
bool trylock(TryLock * tl);
void tryunlock(TryLock * tl);

#endif

#include <trylock.h>
#include <stdbool.h>
#include <stdint.h>

TryLock * tryinit()
{
    TryLock * tl = la_malloc(sizeof(TryLock));
    tl->lock = false;
    return ;
}

bool trylock(TryLock * tl)
{
    return !try_to_lock(&tl->lock);
}

void tryunlock(TryLock * tl)
{
    tl->lock = false;
}

bool itrylock(uint64_t * l){
	return !try_to_lock(l);
}

void itryunlock(uint64_t * l){
	*l=0;
}

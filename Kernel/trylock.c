#include <trylock.h>
#include <stdbool.h>
#include <stdint.h>
typedef struct TryLock {
    uint64_t lock;
} TryLock;

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

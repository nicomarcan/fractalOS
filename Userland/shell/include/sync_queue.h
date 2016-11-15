#ifndef SYNC_QUEUE_H
#define SYNC_QUEUE_H

#include <stdbool.h>
#include <stdint.h>
#include <prodconsgui.h>

typedef struct SQueue SQueue;
SQueue * squeue_init(int max_size);
void senque(SQueue * sq, int64_t a, guiprodcon * pcg);
int64_t sdeque(SQueue * sq, guiprodcon * pcg);
int64_t squeue_size(SQueue * sq);
int64_t squeue_max_size(SQueue * sq);
void squeue_destroy(SQueue * sq);
bool is_empty(SQueue * sq);
#endif

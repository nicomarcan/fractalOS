#ifndef QUEUE_H
#define QUEUE_H

#include <stdint.h>
typedef struct Queue * Queue_p;
int64_t enque(Queue_p q, int64_t t);
int64_t deque(Queue_p q);
int8_t q_is_empty(Queue_p q);
void q_destroy(Queue_p);
int64_t q_size(Queue_p q);
Queue_p new_queue();

#endif

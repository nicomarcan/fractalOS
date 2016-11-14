#ifndef COND_VARIABLE_H
#define COND_VARIABLE_H

#include <c_syscall.h>
#include <queue.h>

typedef struct cond_variable cond_variable;
cond_variable * cond_variable_init();
void cond_variable_destroy(cond_variable * cv);
void cond_variable_wait(cond_variable * c, mutex * m);
void cond_variable_signal(cond_variable * c);

#endif

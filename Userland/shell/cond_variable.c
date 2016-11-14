#include <stdint.h>
#include <cond_variable.h>
#include <stdbool.h>
#include <queue.h>
#include <c_syscall.h>

typedef struct cond_variable {
	Queue_p q;
} cond_variable;

static int64_t reg = false;

cond_variable * cond_variable_init()
{
    cond_variable * cv = malloc(sizeof(cond_variable_init));
	cv->q = new_queue();
    return cv;
}

void cond_variable_destroy(cond_variable * cv)
{
	q_destroy(cv->q);
	free(cv);
}

void cond_variable_wait(cond_variable * c, mutex * m)
{
	/* add thread to cond_variable queue */
	enque(c->q, getPid());
	/* sleep thread and release lock atomically */

	release_lock_and_sleep(m);
	/* re-aquire lock */
	mutex_lock(m);
}

void cond_variable_signal(cond_variable * c)
{
	if (!q_is_empty(c->q)) {
		kill(deque(c->q), 1);
	}
}

#include <stdint.h>
#include <cond_variable.h>

static int64_t reg = false;

cond_variable * cond_variable_init()
{
    cond_variable * cv = malloc(sizeof(cond_variable_init));
    return cv;
}

void cond_variable_wait(cond_variable * c, mutex * m)
{
    mutex_lock(&(c->reg));
    mutex * mc = add_waiting_thread(c);
    mutex_unlock(m);
    mutex_unlock(&(c->reg));
    mutex_lock(mc);
    mutex_unlock(m);
}

void cond_variable_signal(cond_variable * c)
{
    wake_next_waiting_thread(c);
}

static void add_waiting_thread(cond_variable * c)
{
    mutex * mc = malloc(sizeof(mc));
    thread_info * ti = malloc(sizeof(ti));
    mutex_init_lock(mc);
    queue_add(mc);
    mutex_lock(mc);
}

static void wake_next_waiting_thread(cond_variable * c)
{
    mutex * mc = queue_remove(c->q);
    mutex_unlock(mc);
}

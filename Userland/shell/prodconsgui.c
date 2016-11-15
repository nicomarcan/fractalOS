#include <math.h>
#include <libgph.h>
#include <prodconsgui.h>
#include <c_syscall.h>
#define RADIUS 164
#define SQUARESIZE 32

guiprodcon * renderinit(uint64_t max_size, uint64_t radius){
    guiprodcon * ret = malloc(sizeof(guiprodcon));
    ret->next = 0;
    ret->max = max_size;
	ret->m = malloc(sizeof(mutex));
	ret->r = radius;
	mutex_init(ret->m);
    double ang = 0;
    double incr = 2*PI/max_size;
    for(int i = 0; i < max_size; i++, ang+=incr) {
        ret->cosang[i]=cos(ang);
        ret->sinang[i]=sin(ang);
    }
    return ret;
}

void renderenque(guiprodcon * gs){
	mutex_lock(gs->m);
    printCircleFilled2(CENTRE_X + (gs->r)*gs->sinang[gs->next],CENTRE_Y + (gs->r)*gs->cosang[gs->next],SQUARESIZE, RED);
    gs->next++;
	mutex_unlock(gs->m);
}

void renderdeque(guiprodcon * gs){
	mutex_lock(gs->m);
    gs->next--;
    printCircleFilled2(CENTRE_X + (gs->r)*gs->sinang[gs->next],CENTRE_Y + (gs->r)*gs->cosang[gs->next],SQUARESIZE, GREEN);
	mutex_unlock(gs->m);
}

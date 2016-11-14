#include <math.h>
#include <libgph.h>
#include <prodconsgui.h>
#define RADIUS 164
#define SQUARESIZE 32

typedef struct guiprodcon{
	int next;
    int max;
	double cosang[30];
	double sinang[30];
} guiprodcon;

    uint64_t rad = SQUARESIZE ;

guiprodcon * renderinit(uint64_t max_size){
    guiprodcon * ret = malloc(sizeof(guiprodcon));
    ret->next = 0;
    ret->max = max_size;
    double ang = 0;
    double incr = 2*PI/max_size;
    for(int i = 0; i < max_size; i++, ang+=incr) {
        ret->cosang[i]=cos(ang);
        ret->sinang[i]=sin(ang);
            printf(" %d y %d y %d\n",(int)(ang),(int)(ret->cosang[i]*100), (int)(ret->sinang[i]*100));
    }
    return ret;
}

void renderenque(guiprodcon * gs){
    printCircleFilled2(CENTRE_X + RADIUS*gs->sinang[gs->next],CENTRE_Y + RADIUS*gs->cosang[gs->next],rad, RED);
    (gs->next)++;
}

void renderdeque(guiprodcon * gs){
    (gs->next)--;
    printCircleFilled2(CENTRE_X + RADIUS*gs->sinang[gs->next],CENTRE_Y + RADIUS*gs->cosang[gs->next],rad, GREEN);
}

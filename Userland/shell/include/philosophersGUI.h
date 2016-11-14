#ifndef PHILOGUI_H
#define PHILOGUI_H
#include <stdint.h>
#include "philosophers.h"

#define NLINES 3 

typedef struct guistruct{
	State philoState[PHILOMAX];
	int forkState[PHILOMAX];
	mutex m;
	int64_t prev;
	int32_t prevColours[PHILOMAX];
	int64_t initGM;
	double cosang[PHILOMAX];
	double sinang[PHILOMAX];
} guistruct;



void render(uint64_t philos,guistruct * gs);
void renderGM(uint64_t philos,guistruct * gs);
void setPhiloState(int philo, State state,guistruct * gs);
void setForkState(int fork, int owner,guistruct * gs);

#endif

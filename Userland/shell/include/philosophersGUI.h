#ifndef PHILOGUI_H
#define PHILOGUI_H
#include <stdint.h>

#include "philosophers.h"

#define NLINES 3 
void render(uint64_t philos);
void renderGM(uint64_t philos);
void setPhiloState(int philo, State state);
void setForkState(int fork, int owner);

#endif

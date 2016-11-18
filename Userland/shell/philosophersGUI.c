#include <clib.h>
#include <scanf.h>
#include <c_syscall.h>
#include <philosophersGUI.h>
#include <libgph.h>
#include <math.h>
#define SQUARESIZE 32
#define MINSIZE 20
#define Y_DELTA 80
#define X_DELTA 200

char * stateStrings[3] = { "Hungry", "Thinking", "Eating" };

circleprinter cpr = printCircleFilled2;

void render(uint64_t philos,guistruct * gs) {

	if(philos != gs->prev){
		gs->prev = philos;
		clear();
	}
	for(int i = 0; i < philos; i++) {
		printf("Philosopher %d: %s\n", i, stateStrings[gs->philoState[i]]);
		printf("Fork - ");

		if (gs->forkState[i] == -1)
			printf("Free\n");
		else
			printf("Owner %d\n", gs->forkState[gs->forkState[i]]);
	}
	putchar('\n');
	putchar('\n');
}

void renderGM(uint64_t philos,guistruct * gs){
	mutex_lock(&gs->m);
	if(philos != gs->prev){
		gs->prev = philos;
		double incr = 2*PI/philos;
		double ang = 0;
		for(int i=0 ; i<PHILOMAX ; i++ , ang+=incr){
			gs->prevColours[i]=-1;
			gs->cosang[i]=cos(ang);
			gs->sinang[i]=sin(ang);
		}
		clear();
	}
	int32_t colours[PHILOMAX];
	/* 0-RED   -  Hungry */
	/* 1-BLUE  -  Thinking */
	/* 2-GREEN -  Eating */

	for(int i=0;i<philos ; i++){
		switch(gs->philoState[i]){
			case 0:
				colours[i]=RED;
				break;
			case 1:
				colours[i]=BLUE;
				break;
			case 2:
				colours[i]=GREEN;
				break;
			default:
				colours[i]=RED;
				break;
		}
	}
	uint64_t rad = SQUARESIZE + 1*(PHILOINIT-philos);
	uint64_t currx,curry;
	for (int i=0; i<philos ; i++){
		if(gs->prevColours[i] != colours[i]){
			gs->prevColours[i] = colours[i];
			currx = CENTRE_X + (gs->RADIUS)*gs->sinang[i];
			curry = CENTRE_Y + (gs->RADIUS)*gs->cosang[i];
			cpr(currx,curry,rad,colours[i]);
			if(i==0){
				cpr(currx,curry,5,0x000000);		
			}
		}
	}
	sleep(2);
	mutex_unlock(&gs->m);

}

void setPhiloState(int philo, State state,guistruct * gs) {
	gs->philoState[philo] = state;
}

void setForkState(int fork, int owner,guistruct * gs) {
	gs->forkState[fork] = owner;
}

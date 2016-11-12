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
#define RADIUS 164

char * stateStrings[3] = { "Hungry", "Thinking", "Eating" };
State philoState[PHILOMAX];
int forkState[PHILOMAX];
static mutex m;
static uint8_t initGM = 0;
uint64_t prev = -1;

circleprinter cpr = printCircleFilled2;

void render(uint64_t philos) {

	if(philos != prev){
		prev = philos;
		clear();
	}
	for(int i = 0; i < philos; i++) {
		printf("Philosopher %d: %s\n", i, stateStrings[philoState[i]]);
		printf("Fork - ");

		if (forkState[i] == -1)
			printf("Free\n");
		else
			printf("Owner %d\n", forkState[forkState[i]]);
	}
	putchar('\n');
	putchar('\n');
}

void renderGM(uint64_t philos){
	static uint32_t prevColours[PHILOMAX];
	static double cosang[PHILOMAX];
	static double sinang[PHILOMAX];
	if(!initGM){
		mutex_init(&m);
		initGM = 1;
		for(int i=0 ; i<PHILOMAX ; i++){
			prevColours[i]=-1;
		}
	}
	if(philos != prev){
		prev = philos;
		double incr = 2*PI/philos;
		double ang = 0;
		for(int i=0 ; i<PHILOMAX ; i++ , ang+=incr){
			prevColours[i]=-1;
			cosang[i]=cos(ang);
			sinang[i]=sin(ang);
		}
		clear();
	}

	mutex_lock(&m);
	uint32_t colours[PHILOMAX];
	/* 0-RED   -  Hungry */
	/* 1-BLUE  -  Thinking */
	/* 2-GREEN -  Eating */

	for(int i=0;i<philos ; i++){
		switch(philoState[i]){
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
				break;
		}
	}
	uint64_t rad = SQUARESIZE + 1*(PHILOINIT-philos);
	for (int i=0; i<philos ; i++){
		if(prevColours[i] != colours[i]){
			prevColours[i] = colours[i];
			cpr(CENTRE_X + RADIUS*sinang[i],CENTRE_Y + RADIUS*cosang[i],rad,colours[i]);
		}
	}
	sleep(5);
	mutex_unlock(&m);

}

void setPhiloState(int philo, State state) {
	philoState[philo] = state;
}

void setForkState(int fork, int owner) {
	forkState[fork] = owner;
}

#include <clib.h>
#include <scanf.h>
#include <c_syscall.h>
#include <philosophersGUI.h>
#include <libgph.h>
#include <math.h>
#define SQUARESIZE 64
#define Y_DELTA 80
#define X_DELTA 200
#define RADIUS 164

char * stateStrings[3] = { "Hungry", "Thinking", "Eating" };
State philoState[PHILOMAX];
int forkState[PHILOMAX];
static mutex m;
static uint8_t initGM = 0;
uint64_t prev = -1;
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

	if(!initGM){
		mutex_init(&m);
		initGM = 1;
	}
	if(philos != prev){
		prev = philos;
		clear();
	}

	mutex_lock(&m);
	uint32_t colours[PHILOMAX];
	uint64_t perline = philos/NLINES;
	perline += philos%NLINES != 0 ? 1:0;
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

	double incr = 2*PI/philos;
	double ang = 0;
	uint64_t sqsize = SQUARESIZE + 4*(PHILOINIT-philos);
	for (int i=0; i<philos ; i++ , ang+=incr){
		printSquare2(CENTRE_X + RADIUS*sin(ang),CENTRE_Y + RADIUS*cos(ang),sqsize,sqsize,colours[i]);
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

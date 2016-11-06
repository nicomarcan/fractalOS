#include <clib.h>
#include <scanf.h>
#include <c_syscall.h>
#include <philosophersGUI.h>
#include <libgph.h>
#define SQUARESIZE 64
#define Y_DELTA 80
#define X_DELTA 200

char * stateStrings[3] = { "Hungry", "Thinking", "Eating" };
State philoState[PHILOCOUNT];
int forkState[PHILOCOUNT];
static mutex m;
static uint8_t initGM = 0;

void render() {
	
	
	for(int i = 0; i < PHILOCOUNT; i++) {
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

void renderGM(){
	
	if(!initGM){
		mutex_init(&m);
		initGM = 1;
	}
	
	mutex_lock(&m);
	/* Just a demo */
	uint32_t colours[PHILOCOUNT];
	
	/* 0-RED   -  Hungry */
	/* 1-BLUE  -  Thinking */
	/* 2-GREEN -  Eating */
	
	for(int i=0;i<PHILOCOUNT ; i++){
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
	
	uint64_t ypos = CENTRE_Y - Y_DELTA;
	uint64_t xpos = CENTRE_X - X_DELTA;
	
	/* 3 per line */
	for (int i=0; i<PHILOCOUNT ; i++){
		if(i%3 == 0 && i!=0){
			xpos += X_DELTA;
			ypos = CENTRE_Y - Y_DELTA;
		}
		printSquare2(xpos,ypos,SQUARESIZE,SQUARESIZE,colours[i]);
		ypos += Y_DELTA;
	}
	/*
	printSquare2(CENTRE_X,CENTRE_Y-80,64,64,colours[0]);
	printSquare2(CENTRE_X,CENTRE_Y,64,64,colours[1]);
	printSquare2(CENTRE_X,CENTRE_Y+80,64,64,colours[2]);
	*/
	sleep(10);
	mutex_unlock(&m);
	
}

void setPhiloState(int philo, State state) {
	philoState[philo] = state;
}

void setForkState(int fork, int owner) {
	forkState[fork] = owner;
}

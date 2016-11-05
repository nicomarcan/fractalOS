#include <clib.h>
#include <scanf.h>
#include <c_syscall.h>
#include <philosophersGUI.h>
#include <libgph.h>

char * stateStrings[3] = { "Hungry", "Thinking", "Eating" };
State philoState[PHILOCOUNT];
int forkState[PHILOCOUNT];

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
	
	
	
	/* Just a demo */
	
	uint32_t colours[2];
	
	/* 0-RED   -  Hungry */
	/* 1-BLUE  -  Thinking */
	/* 2-GREEN -  Eating */
	
	/*
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
	printSquare2(CENTRE_X-50,CENTRE_Y,64,64,colours[0]);
	printSquare2(CENTRE_X+50,CENTRE_Y,64,64,colours[1]);
	*/
	
}

void setPhiloState(int philo, State state) {
	philoState[philo] = state;
}

void setForkState(int fork, int owner) {
	forkState[fork] = owner;
}

#include <clib.h>
#include <scanf.h>
#include <c_syscall.h>
#include <philosophersGUI.h>

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
}

void setPhiloState(int philo, State state) {
	philoState[philo] = state;
}

void setForkState(int fork, int owner) {
	forkState[fork] = owner;
}

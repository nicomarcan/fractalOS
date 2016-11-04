#include <clib.h>
#include <scanf.h>
#include <c_syscall.h>
#include <philosophers.h>
#include <philosophersGUI.h>

int left(int i);
int right(int i);
void * philosopher(uint64_t argc, uint8_t ** argv);
void takeForks(int id);
void putForks(int id);
void test(int i);
int randRange(int min, int max);
extern int forkState[PHILOCOUNT];

State state[PHILOCOUNT];

uint64_t mutex;
uint64_t semaphores[PHILOCOUNT];
int philosopherId[PHILOCOUNT];

void * philosopher(uint64_t argc, uint8_t ** argv) {
	while(1) {
		//Think
		//sleep(10);	
		sleep(randRange(10, 20));

		takeForks(argc);

		//Eat
		//sleep(10);
		sleep(randRange(10, 20));

		putForks(argc);
	}
}

void takeForks(int id) {
	mutex_lock(&mutex);				//Crit zone

	//Set state
	state[id] = Hungry;
	setPhiloState(id, Hungry);
	render();

	test(id);						//Try to acquire forks
	mutex_unlock(&mutex);			//Crit zone exit
	mutex_lock(&semaphores[id]);	//Locks if forks not acquired
}

void putForks(int id) {
	mutex_lock(&mutex);				//Crit zone

	//Set state
	state[id] = Thinking;
	//Think and release forks
	setPhiloState(id, Thinking);
	setForkState(left(id), -1);
	setForkState(id, -1);
	render();

	test(left(id));							//Try to acquire forks for left
	test(right(id));						//Try to acquire forks for right
	mutex_unlock(&mutex);			//Crit zone exit
}

void test(int id) {
	if (state[id] == Hungry &&				//Philosopher is hungry
		state[left(id)] != Eating &&		//Both philosophers at
		state[right(id)] != Eating) {		//left and right are not eating

		//Set state
		state[id] = Eating;					//Philosopher can eat!
		//Take forks and eat
		setPhiloState(id, Eating);
		setForkState(left(id), id);
		setForkState(id, id);
		render();

		mutex_unlock(&semaphores[id]);	//Forks acquired, unlock
	}
}

int64_t philosophers(uint64_t argc, uint8_t ** argv) {
	mutex=0;
	for (int i = 0; i < PHILOCOUNT; i++) {
		forkState[i]=-1;
		semaphores[i]=1;		//Philosophers start not having
	}											//ownership of the forks

	
	for (int i = 0; i < PHILOCOUNT; i++) {
		philosopherId[i] = i;
		state[i] = Thinking;
		Args * args = malloc(sizeof(Args));
		args->argc=i;
		args->fg=0;
		fkexec(philosopher,"philo",args);
		free(args);
	}

	printf("running\n");
	getchar();

	exit();
}

int left(int i) {
	return (i + PHILOCOUNT - 1) % PHILOCOUNT;
}

int right(int i) {
	return (i + 1) % PHILOCOUNT;
}

int randRange(int min, int max) {
	return rand() % (max - min) + min;
}

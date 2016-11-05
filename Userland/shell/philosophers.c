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
extern int forkState[PHILOCOUNT];

State state[PHILOCOUNT];

mutex m;
mutex semaphores[PHILOCOUNT];
int philosopherId[PHILOCOUNT];
uint64_t philosopherPID[PHILOCOUNT];
uint8_t paused=0;
void * philosopher(uint64_t argc, uint8_t ** argv) {
	while(1) {
		if(paused){
			_wait();
		}
		//Think
		//sleep(10);	
		sleep(35);

		takeForks(argc);

		//Eat
		//sleep(10);
		sleep(35);

		putForks(argc);
	}
}

void takeForks(int id) {
	mutex_lock(&m);				//Crit zone

	//Set state
	state[id] = Hungry;
	setPhiloState(id, Hungry);
	render();

	test(id);						//Try to acquire forks
	mutex_unlock(&m);			//Crit zone exit
	mutex_lock(&semaphores[id]);	//Locks if forks not acquired
}

void putForks(int id) {
	mutex_lock(&m);				//Crit zone

	//Set state
	state[id] = Thinking;
	//Think and release forks
	setPhiloState(id, Thinking);
	setForkState(left(id), -1);
	setForkState(id, -1);
	render();

	test(left(id));							//Try to acquire forks for left
	test(right(id));						//Try to acquire forks for right
	mutex_unlock(&m);			//Crit zone exit
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
	mutex_init(&m);
	for (int i = 0; i < PHILOCOUNT; i++) {
		forkState[i]=-1;
		mutex_init(&semaphores[i]);		//Philosophers start not having
	}											//ownership of the forks

	
	for (int i = 0; i < PHILOCOUNT; i++) {
		philosopherId[i] = i;
		state[i] = Thinking;
		Args * args = malloc(sizeof(Args));
		args->argc=i;
		args->fg=0;
		philosopherPID[i] = fkexec(philosopher,"philo",args);
		free(args);
	}

	printf("running\n");
	
	uint8_t run = 1;
	uint8_t c;
	while(run){
		c=getchar();
		mutex_lock(&m);
		switch(c){
			case 'q':
				/* quit */ 
				for(int i = 0; i<PHILOCOUNT ; i++){
					kill(philosopherPID[i],0);
				}
				run = 0;
				break;
			case 'p':
				/* pause */
				paused = 1;
				break;
			case 'r':
				/* resume */
				if(paused){
					paused=0;
					for(int i = 0; i<PHILOCOUNT ; i++){
						kill(philosopherPID[i],1);
					}
				}
				break;
			default:
				break;
		}
		mutex_unlock(&m);
	}
	exit();
}

int left(int i) {
	return (i + PHILOCOUNT - 1) % PHILOCOUNT;
}

int right(int i) {
	return (i + 1) % PHILOCOUNT;
}

#include <clib.h>
#include <scanf.h>
#include <c_syscall.h>
#include <philosophers.h>
#include <philosophersGUI.h>

typedef void (*voidfunc)(uint64_t);
static void insertPhilo(uint64_t id);
static void removePhilo();

int left(int i);
int right(int i);
void * philosopher(uint64_t argc, uint8_t ** argv);
void takeForks(int id);
void putForks(int id);
void test(int i);
extern int forkState[PHILOMAX];

State state[PHILOMAX];

mutex m;
mutex semaphores[PHILOMAX];
int philosopherId[PHILOMAX];
uint64_t philosopherPID[PHILOMAX];
static uint8_t paused=0;
static uint8_t run = 1;
voidfunc renderf=renderGM;

static uint64_t PHILOCOUNT = PHILOINIT;

void * philosopher(uint64_t argc, uint8_t ** argv) {
	while(1) {
		if(paused){
			_wait();
		}
		//Think
		sleep(20);

		takeForks(argc);

		//Eat
		sleep(20);

		putForks(argc);
	}
	exit();
}

void takeForks(int id) {
	mutex_lock(&m);				//Crit zone

	//Set state
	state[id] = Hungry;
	setPhiloState(id, Hungry);
	renderf(PHILOCOUNT);

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
	renderf(PHILOCOUNT);

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
		renderf(PHILOCOUNT);

		mutex_unlock(&semaphores[id]);	//Forks acquired, unlock
	}
}

int64_t philosophers(uint64_t argc, uint8_t ** argv) {
	paused = 0;
	run = 1;
	mutex_init(&m);
	for (int i = 0; i < PHILOCOUNT; i++) {
		mutex_init(&semaphores[i]);		//Philosophers start not having
	}											//ownership of the forks


	for (int i = 0; i < PHILOCOUNT; i++) {
		insertPhilo(i);
	}

	printf("running\n");

	uint8_t c;
	while(run){
		c=getchar();
		switch(c){
			case 'q':
				/* quit */
				if(paused){
					mutex_lock(&m);
					printf("Locked mutex\n");
					for(int i = 0; i<PHILOCOUNT ; i++){
						mutex_destroy(&semaphores[i]);
						kill(philosopherPID[i],0);
					}
					run = 0;
					paused = 1;
					printf("Exiting\n");
					mutex_destroy(&m);
					exit();
				} else {
					printf("Pause first\n");
				}
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
			case 'w':
				/* add philosopher */
				if(PHILOCOUNT<PHILOMAX){
					insertPhilo(PHILOCOUNT);
					PHILOCOUNT ++ ;
				} else {
					printf("Maximum of %d philosophers reached\n",PHILOMAX);
				}
				break;
			case 's':
				/* remove philosopher */
				if(PHILOCOUNT>2){
					mutex_lock(&m);
					removePhilo();
					mutex_unlock(&m);
				} else {
					printf("Minimum of 2 philosophers reached\n");
				}
				break;
			default:
				break;
		}
	}
	exit();
}

static void insertPhilo(uint64_t id){
	philosopherId[id] = id;
	state[id] = Thinking;
	Args * args = malloc(sizeof(Args));
	args->argc=id;
	args->fg=0;
	philosopherPID[id] = fkexec(philosopher,"philo",args);
	free(args);
}

static void removePhilo(){
	PHILOCOUNT -- ;
	mutex_destroy(&semaphores[PHILOCOUNT]);
	kill(philosopherPID[PHILOCOUNT],0);
}

int left(int i) {
	return (i + PHILOCOUNT - 1) % PHILOCOUNT;
}

int right(int i) {
	return (i + 1) % PHILOCOUNT;
}

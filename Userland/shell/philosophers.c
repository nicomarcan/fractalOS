#include <clib.h>
#include <scanf.h>
#include <c_syscall.h>
#include <philosophers.h>
#include <philosophersGUI.h>

typedef struct mainstruct {
	State state[PHILOMAX];		
	mutex m;
	mutex semaphores[PHILOMAX];
	int philosopherId[PHILOMAX];
	uint64_t philosopherPID[PHILOMAX];
	uint8_t paused ;
	uint8_t run ;
	uint64_t PHILOCOUNT; 
} mainstruct;


typedef void (*voidfunc)(uint64_t,guistruct*);
static void insertPhilo(uint64_t id,guistruct * gs,mainstruct * ms);
static void removePhilo(mainstruct * ms);
static void initStructs(guistruct * gs,mainstruct * ms);


int left(int i,mainstruct * ms);
int right(int i,mainstruct * ms);
void * philosopher(uint64_t argc, uint8_t ** argv);
void takeForks(int id,guistruct * gs,mainstruct * ms);
void putForks(int id,guistruct * gs,mainstruct * ms);
void test(int i,guistruct * gs,mainstruct * ms);

voidfunc renderf=renderGM;


void * philosopher(uint64_t argc, uint8_t ** argv) {
	guistruct * gs = argv[0];
	mainstruct * ms = argv[1];
	while(1) {
		if(ms->paused){
			_wait();
		}
		//Think
		sleep(20);

		takeForks(argc,gs,ms);

		//Eat
		sleep(20);

		putForks(argc,gs,ms);
	}
	exit();
}

void takeForks(int id,guistruct * gs,mainstruct * ms) {
	mutex_lock(&ms->m);				//Crit zone

	//Set state
	ms->state[id] = Hungry;
	setPhiloState(id, Hungry,gs);
	renderf(ms->PHILOCOUNT,gs);

	test(id,gs,ms);						//Try to acquire forks
	mutex_unlock(&ms->m);			//Crit zone exit
	mutex_lock(&ms->semaphores[id]);	//Locks if forks not acquired
}

void putForks(int id,guistruct * gs,mainstruct * ms) {
	mutex_lock(&ms->m);				//Crit zone

	//Set state
	ms->state[id] = Thinking;
	//Think and release forks
	setPhiloState(id, Thinking,gs);
	setForkState(left(id,ms), -1,gs);
	setForkState(id, -1,gs);
	renderf(ms->PHILOCOUNT,gs);

	test(left(id,ms),gs,ms);							//Try to acquire forks for left
	test(right(id,ms),gs,ms);						//Try to acquire forks for right
	mutex_unlock(&ms->m);			//Crit zone exit
}

void test(int id,guistruct * gs,mainstruct * ms) {
	if (ms->state[id] == Hungry &&				//Philosopher is hungry
		ms->state[left(id,ms)] != Eating &&		//Both philosophers at
		ms->state[right(id,ms)] != Eating) {		//left and right are not eating

		//Set state
		ms->state[id] = Eating;					//Philosopher can eat!
		//Take forks and eat
		setPhiloState(id, Eating,gs);
		setForkState(left(id,ms), id,gs);
		setForkState(id, id,gs);
		renderf(ms->PHILOCOUNT,gs);

		mutex_unlock(&ms->semaphores[id]);	//Forks acquired, unlock
	}
}

int64_t philosophers(uint64_t argc, uint8_t ** argv) {
	guistruct gs;
	mainstruct ms;
	initStructs(&gs,&ms);

	for (int i = 0; i < ms.PHILOCOUNT; i++) {
		insertPhilo(i,&gs,&ms);
	}

	printf("running\n");

	uint8_t c;
	while(ms.run){
		c=getchar();
		switch(c){
			case 'q':
				/* quit */
				if(ms.paused){
					mutex_lock(&ms.m);
					printf("Locked mutex\n");
					for(int i = 0; i<ms.PHILOCOUNT ; i++){
						mutex_destroy(&ms.semaphores[i]);
						kill(ms.philosopherPID[i],0);
					}
					ms.run = 0;
					ms.paused = 1;
					printf("Exiting\n");
					mutex_destroy(&ms.m);
					exit();
				} else {
					printf("Pause first\n");
				}
				break;
			case 'p':
				/* pause */
				ms.paused = 1;
				break;
			case 'r':
				/* resume */
				if(ms.paused){
					ms.paused=0;
					for(int i = 0; i<ms.PHILOCOUNT ; i++){
						kill(ms.philosopherPID[i],1);
					}
				}
				break;
			case 'w':
				/* add philosopher */
				if(ms.PHILOCOUNT<PHILOMAX){
					insertPhilo(ms.PHILOCOUNT,&gs,&ms);
					ms.PHILOCOUNT ++ ;
				} else {
					printf("Maximum of %d philosophers reached\n",PHILOMAX);
				}
				break;
			case 's':
				/* remove philosopher */
				if(ms.PHILOCOUNT>2){
					mutex_lock(&ms.m);
					removePhilo(&ms);
					mutex_unlock(&ms.m);
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

static void initStructs(guistruct * gs,mainstruct * ms){
	gs->prev = -1;
	gs->initGM = 0;
	for(int i=0;i < PHILOINIT ; i++){
		mutex_init(&ms->semaphores[i]);
	}
	ms->paused=0;
	ms->run=1;
	ms->PHILOCOUNT=PHILOINIT;
	mutex_init(&ms->m);
}

static void insertPhilo(uint64_t id,guistruct * gs,mainstruct * ms){
	ms->philosopherId[id] = id;
	ms->state[id] = Thinking;
	setPhiloState(id, Thinking,gs);
	Args * args = malloc(sizeof(Args));
	args->argc=id;
	args->argv=malloc(2*sizeof(void*));
	args->argv[0]=gs;
	args->argv[1]=ms;
	args->fg=0;
	ms->philosopherPID[id] = fkexec(philosopher,"philo",args);
	free(args);
}

static void removePhilo(mainstruct * ms){
	ms->PHILOCOUNT -- ;
	mutex_destroy(&ms->semaphores[ms->PHILOCOUNT]);
	kill(ms->philosopherPID[ms->PHILOCOUNT],0);
}

int left(int i,mainstruct * ms) {
	return (i + ms->PHILOCOUNT - 1) % ms->PHILOCOUNT;
}

int right(int i,mainstruct * ms) {
	return (i + 1) % ms->PHILOCOUNT;
}

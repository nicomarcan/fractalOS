#include <sync_queue.h>
#include <stdbool.h>
#include <c_syscall.h>
#include <scanf.h>
#include <clib.h>
#include <stdbool.h>
#define MIN_SPEED 10

static void * producer(uint64_t argc, uint8_t ** argv) {
	SQueue * pq = (SQueue *) argv[0];
	int * prod_speed = (int *) argv[2];
	int i = 0;
	while(1) {
		//Think
		sleep(*prod_speed);
		senque(pq, i);
		printf("Producido %d. Ahora hay %d/%d items en la cola.\n", i, squeue_size(pq), squeue_max_size(pq));
		i++;
	}
	exit();
}

static void * consumer(uint64_t argc, uint8_t ** argv) {
	SQueue * pq = (SQueue *) argv[0];
	int * cons_speed = (int *) argv[1];
	int i = 0;

	while(1) {
		//Think
		sleep(*cons_speed);
		i = sdeque(pq);
		printf("Consumido %d. Ahora hay %d/%d items en la cola.\n", i, squeue_size(pq), squeue_max_size(pq));
	}
	exit();
}

int64_t prod_con(int64_t argc, int64_t * argv[])
{
	int max_capacity,* cons_speed, * prod_speed;

	if (argc <= 0) {
		/*
		printf("Por favor indique la capacidad de la cola.\n");
		exit();
		*/
		max_capacity = 1000;
	}
	else {
		sscanf(argv[0], "%d", &max_capacity);
	}

	SQueue * sq = squeue_init(max_capacity);
	Args * arg = malloc(sizeof(Args));
	cons_speed = malloc(sizeof(int));
	prod_speed = malloc(sizeof(int));

	*cons_speed = 5;
	*prod_speed = 5;
	arg->argc = 1;
	arg->argv = malloc(3 * sizeof(SQueue *));
	arg->argv[0] = sq;
	arg->argv[1] = cons_speed;
	arg->argv[2] = prod_speed;
	int prod_pid = fkexec(producer, "producer", arg);
	int cons_pid = fkexec(consumer, "consumer", arg);

	int8_t c, end = false;
	while(!end) {
		c = getchar();
		switch(c) {
			case 'q':
				kill(prod_pid, 0);
				kill(cons_pid, 0);
				free(arg);
				squeue_destroy(sq);
				free(cons_speed);
				free(prod_speed);
				end = true;
				break;
			case 'a':
				/* consumption speed up */
				if (*cons_speed > 0){
					(*cons_speed)--;
					printf("La velocidad del consumidor aumento. Ahora es %d\n", *cons_speed );
				}
				else {
					printf("La velocidad del consumidor esta al maximo.\n");
				}
				break;
			case 'z':
				/* consumption speed down */
				if (*cons_speed < MIN_SPEED ){
					(*cons_speed)++;
					printf("La velocidad del consumidor diminuyo. Ahora es %d\n", *cons_speed );
				}
				else {
					printf("La velocidad del consumidor esta al minimo.\n");
				}
				break;
			case 's':
				/* production speed up */
				if (*prod_speed > 0  ){
					(*prod_speed)--;
					printf("La velocidad del productor aumento. Ahora es %d\n", *prod_speed );
				}
				else {
					printf("La velocidad del productor esta al maximo.\n");
				}
				break;
			case 'x':
				/* production speed down */
				if (*prod_speed < MIN_SPEED ){
					(*prod_speed)++;
					printf("La velocidad del productor disminuyo. Ahora es %d\n", *prod_speed );
				}
				else {
					printf("La velocidad del productor esta al minimo.\n");
				}
				break;
			default:
				break;
		}
	}
	exit();
}

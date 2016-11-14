#include <command.h>
#include <clib.h>
#include <printf.h>
#include <stdint.h>
#include <c_syscall.h>
#include <fifo.h>
#include <c_string.h>

/* simple debug hello, world */
int64_t hello_world(uint64_t argc, uint8_t * argv[]) {
  printf("hello, world\n");
  exit();
}

int64_t fkbomb(uint64_t argc, uint8_t * argv[]) {
  Args a;
  a.argc = 0;
  a.fg = 0;
  printf("fork!");
  fkexec(fkbomb,"fkbomb",&a);
  fkexec(fkbomb,"fkbomb",&a);
  exit();
}

/* waits, if not prints messages */
int64_t waiter(uint64_t argc, uint8_t * argv[]) {
	_wait();
	for(;;) {
		printf("hola");
	}
	exit();
}

int64_t fg(uint64_t argc, uint8_t * argv[]){
	if(argc!=1){
		printf("Must pass target pid\n");
		exit();
	}
	_fg(c_atoi(argv[0]));
	exit();
}

int64_t mutextest(uint64_t argc, uint8_t * argv[]) {
	mutex m;
	mutex_init(&m);
	mutex_lock(&m);
	if(m.m == 1){
		printf("Locked successfully\n");
	} else {
		printf("Lock failed\n");
	}
	mutex_unlock(&m);
	if(m.m == 0){
		printf("Unlocked successfully\n");
	} else {
		printf("Unlock failed\n");
	}
	mutex_destroy(&m);
	exit();
}

/* prints current processes */
int64_t _ps(uint64_t argc, uint8_t * argv[]){
	ProcessInfo * pi = (ProcessInfo *)ps();
	printf("Number of processes: %d\n",pi->process_count);
	uint8_t * status;
	for( int i=0; i<pi->process_count ; i++){
		switch((pi->status)[i]){
			case 0:
				status = "running";
				break;
			case 1:
				status = "sleeping";
				break;
			case 2:
				status = "ready";
				break;

			default:
				status = "unknown";
				break;
		}
		printf("%d ---- %s ---- %s\n",(pi->PIDs)[i],(pi->descrs)[i],status);
	}
	free(pi->PIDs);
	free(pi->descrs);
	free(pi->status);
	free(pi);
	exit();
}

int64_t _kill(uint64_t argc,uint8_t * argv[]){
	if(argc!=2){
		printf("Must pass a pid and kill mode as argument\n");
		exit();
	}
	kill(c_atoi(argv[0]),c_atoi(argv[1]));
	exit();
}

int64_t infiloop(uint64_t argc,uint8_t * argv[]){
	for(;;);
	exit();
}

/* prints current time */
int64_t curr_time(uint64_t argc, uint8_t * argv[]) {
  static uint8_t str [50];
  print_time(str);
  exit();
}

/* prints input, escapes with "" */
int64_t echo(uint64_t argc, uint8_t * argv[]) {
  for (int i=0; i < argc; i++) {
    printf(argv[i]);
  }
  putchar('\n');
  exit();
}


void ipcs(){
        OPENED_FIFOS of;
        int i = 0;
        s_ipcs(&of);
        putchar('\n');
        printf("Fifos Abiertos: %d\n",of.size);
        for(;i<of.size;i++){
                printf("%d-> Direccion asociada: %s\n",i,of.fifos[i] );
        }
         exit();
}
int64_t read_fifoc(uint64_t argc, uint8_t * argv[]) {
  char buf2[100] ;
  int64_t r;
  if(argc < 2){
    printf("Ingrese una direccion y una cantidad a leer menor a 100" );
    putchar('\n');
    exit();
  }
  if(c_atoi(argv[1]) > 100){
         printf("Ingrese una cantidad a leer menor a 100" );
         putchar('\n');
         exit();
  }
  r = open_fifo(argv[0],READ);
  if(r < 2){
    printf("failed" );
    putchar('\n');
  }
  if(read(r,(uint8_t *)buf2,c_atoi(argv[1])) < 0){
    printf("El fifo no está inicializado" );
    putchar('\n');
    exit();
  }
  printf(buf2);
  putchar('\n');
  close_fifo(argv[0],READ);
  exit();
}

int64_t mkfifoc(uint64_t argc, uint8_t * argv[]) {
  int64_t ans;
  if(argc < 1){
    printf("Ingrese una direccion" );
    putchar('\n');
    exit();
  }
  ans = mkfifo(argv[0]);
  if (ans < 1){
    printf("Esa direccion ya fue usada");
      putchar('\n');
    exit();
  }

  printf("El fifo se creo con exito");
    putchar('\n');
  exit();
}

int64_t rmfifoc(uint64_t argc, uint8_t * argv[]) {
  int64_t ans;
  if(argc < 1){
    printf("Ingrese una direccion" );
    putchar('\n');
    exit();
  }
  rmfifo(argv[0]);
  exit();
}


int64_t write_fifoc(uint64_t argc, uint8_t * argv[]) {
  int64_t ans;
  int64_t w;
  if(argc < 2){
    printf("Ingrese una direccion y un string" );
    putchar('\n');
    exit();
  }
  if(c_strlen(argv[1]) > 100){
          printf("La longitud del string debe ser menor a 100" );
          putchar('\n');
          exit();
  }
  w = open_fifo(argv[0],WRITE);
  if(write < 2){
    printf("failed" );
    putchar('\n');
  }
  if (write(w,argv[1],c_strlen(argv[1])) < 0){
    printf("failed" );
    putchar('\n');
  }
  close_fifo(argv[0],WRITE);
  exit();

}

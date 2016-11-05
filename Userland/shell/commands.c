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
	for( int i=0; i<pi->process_count ; i++){
		printf("%d ---- %s ---- %s\n",(pi->PIDs)[i],(pi->descrs)[i],(pi->status)[i]);
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

int64_t read_fifoc(uint64_t argc, uint8_t * argv[]) {
  char buf2[100] ;
  if(argc < 2){
    printf("Ingrese una direccion y una cantidad a leer menor a 100" );
    putchar('\n');
    exit();
  }

  if(read_fifo(argv[0],(uint8_t *)buf2,argv[1][0]-'0') < 0){
    printf("failed" );
    putchar('\n');
    exit();
  }
  printf(buf2);
  putchar('\n');
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
    printf("failed");
      putchar('\n');
    exit();
  }
  printf("succeed");
    putchar('\n');
  exit();
}

int64_t write_fifoc(uint64_t argc, uint8_t * argv[]) {
  int64_t ans;
  if(argc < 2){
    printf("Ingrese una direccion y un string" );
    putchar('\n');
    exit();
  }

  putchar('\n');
  if (write_fifo(argv[0],argv[1],c_strlen(argv[1])) < 0){
    printf("failed" );
    putchar('\n');
  }

  exit();

}

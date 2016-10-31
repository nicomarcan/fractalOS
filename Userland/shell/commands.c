#include <command.h>
#include <clib.h>
#include <printf.h>
#include <stdint.h>
#include <c_syscall.h>

/* simple debug hello, world */
int64_t hello_world(uint64_t argc, uint8_t * argv[]) {
  printf("hello, world\n");
  exit();
}

/* prints current processes */
int64_t _ps(uint64_t argc, uint8_t * argv[]){
	ProcessInfo * pi = (ProcessInfo *)ps();
	printf("Number of processes: %d\n",pi->process_count);
	for( int i=0; i<pi->process_count ; i++){
		printf("%d ---- %s\n",(pi->PIDs)[i],(pi->descrs)[i]);
	}
	free(pi->PIDs);
	free(pi->descrs);
	free(pi);
	exit();
}

int64_t _kill(uint64_t argc,uint8_t * argv[]){
	if(argc==0){
		printf("Must pass a pid as argument\n");
		exit();
	}
	kill(c_atoi(argv[0]));
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

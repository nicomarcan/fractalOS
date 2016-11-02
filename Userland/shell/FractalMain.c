#include <stdint.h>
#include <clib.h>
#include <Point.h>
#include <Square.h>
#include <c_string.h>
#include <c_syscall.h>
#include <Stack.h>
#define CENTRE_X 384
#define CENTRE_Y 512

void initializeFractal(Square * s);
Stack * step();

int fractalMain(uint64_t argc, uint8_t** argv) {
	uint32_t size=256;
	uint8_t i=7;

	if(argc){
		if(argc!=1) return 1;
		if(c_strcmp("big",argv[0])==0){
			size=512;
			i=8;
		} else if(c_strcmp("small",argv[0])==0){
		} else {
			return 1;
		}
		exit();
	}

	initializeFractal(newSquare(CENTRE_X,CENTRE_Y,size,size));

	Stack * s;
	while(i){
		sleep(18*2);
		s = step();
		i--;
	}
	sleep(18*2);
	clear_screen();
	Square * sq;
	while(!isEmpty(s)){
		sq = pop(s);
		deleteSquare(sq);
	}
	free(s);

	exit();
}

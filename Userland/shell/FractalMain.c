#include <stdint.h>
#include <clib.h>
#include <Point.h>
#include <Square.h>
#include <c_string.h>
#include <c_syscall.h>
#include <Stack.h>
#include <libgph.h>
#include <fractal.h>
#define CENTRE_X 384
#define CENTRE_Y 512

void initializeFractal(Square * s,status * st);
Stack * step(status * st);

int fractalMain(uint64_t argc, uint8_t** argv) {
	uint32_t size=256;
	uint8_t i=7;
	status st;
	st.mode=0;
	st.colour = RED;
	initializeFractal(newSquare(CENTRE_X,CENTRE_Y,size,size),&st);

	Stack * s;
	while(i){
		sleep(18*2);
		s = step(&st);
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

	wkexit();
}

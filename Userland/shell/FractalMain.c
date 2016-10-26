#include <stdint.h>
#include <clib.h>
#include <Point.h>
#include <Square.h>
#include <c_string.h>
#define CENTRE_X 384
#define CENTRE_Y 512

void initializeFractal(Square * s);
void step();

int fractalMain(uint64_t argc, uint8_t** argv) {
	uint32_t size=512;
	uint8_t i=8;
	
	if(argc){
		if(argc!=1) return 1;
		if(c_strcmp("big",argv[0])==0){
		} else if(c_strcmp("small",argv[0])==0){
			size=256;
			i=7;
		} else {
			return 1;
		}
	}
	
	initializeFractal(newSquare(CENTRE_X,CENTRE_Y,size,size));
	
	while(i){
		sleep(18*2);
		step();
		i--;
	}
	sleep(18*2);
	clear_screen();
	
	return 0;
}

#include <stdint.h>
#include <asmlib.h>
#include <Point.h>
#include <Square.h>

extern char bss;
extern char endOfBinary;


void * memset(void * destiny, int32_t c, uint64_t length);

int main() {
	int i=8;
	
	//Clean BSS
	memset(&bss, 0, &endOfBinary - &bss);
	
	initializeFractal(newSquare(512-128,384+128,512,512));
	
	while(i){
		sleep(18*3);
		step();
		i--;
	}
	
	return 0;
}

void * memset(void * destiation, int32_t c, uint64_t length) {
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destiation;

	while(length--)
		dst[length] = chr;

	return destiation;
}

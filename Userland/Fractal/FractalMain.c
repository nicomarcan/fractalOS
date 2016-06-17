#include <stdint.h>
#include <asmlib.h>
#include <Point.h>
#include <Square.h>
#define CENTRE_X 384
#define CENTRE_Y 512

extern char bss;
extern char endOfBinary;


void * memset(void * destiny, int32_t c, uint64_t length);

int main(uint64_t argc, uint8_t** argv) {
	uint32_t size=512;
	uint8_t i=8;
	
	//Clean BSS
	memset(&bss, 0, &endOfBinary - &bss);
	
	if(argc){
		if(argc!=1) return 1;
		if(strcmp("big",argv[0])==0){
		} else if(strcmp("small",argv[0])==0){
			size=256;
		} else {
			return 1;
		}
	}
	
	initializeFractal(newSquare(CENTRE_X,CENTRE_Y,size,size));
	
	while(i){
		sleep(18*3);
		step();
		i--;
	}
	clearscreen();
	
	return 0;
}

/*
 * http://stackoverflow.com/questions/20004458/optimized-strcmp-implementation
 */
int strcmp(const char* s1, const char* s2)
{
    while(*s1 && (*s1==*s2)){
        s1++,s2++;
	}
    return *(const unsigned char*)s1-*(const unsigned char*)s2;
}

void * memset(void * destiation, int32_t c, uint64_t length) {
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destiation;

	while(length--)
		dst[length] = chr;

	return destiation;
}

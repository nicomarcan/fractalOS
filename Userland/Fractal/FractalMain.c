#include <stdint.h>
#include <asmlib.h>

extern char bss;
extern char endOfBinary;


void * memset(void * destiny, int32_t c, uint64_t length);


char * main() {
	int i,j;
	
	//Clean BSS
	memset(&bss, 0, &endOfBinary - &bss);
	
	for(i=100;i<200;i++){
		for(j=100;j<200;j++){
			pixel(i,j,0x0000FF00);
		}
	}
	
	return "hola";
}


void * memset(void * destiation, int32_t c, uint64_t length) {
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destiation;

	while(length--)
		dst[length] = chr;

	return destiation;
}

#include <stdint.h>
#include <asmlib.h>
#include <Point.h>
#include <Square.h>

extern char bss;
extern char endOfBinary;


void * memset(void * destiny, int32_t c, uint64_t length);
static void printSquareS(Square * s);

int main() {
	int i;
	
	//Clean BSS
	memset(&bss, 0, &endOfBinary - &bss);
	
//	initializeFractal(newPoint(1024/2,768/2),256);
	
	Square * s = newSquare(100,100,100,100);
	
	printSquareS(s);
	
	return 0;
}

static void printSquareS(Square * s){
		int i,j;
		Point * p = topLeftCorner(s);
		for(i=p->x;i<s->width;i++){
			for(j=p->y;j<s->height;j++){
				pixel(i,j,0x00FFFF00);
			}
		}
	}

void * memset(void * destiation, int32_t c, uint64_t length) {
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destiation;

	while(length--)
		dst[length] = chr;

	return destiation;
}

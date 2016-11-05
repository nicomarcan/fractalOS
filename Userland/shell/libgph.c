#include <libgph.h>
#include <Point.h>
#include <c_syscall.h>

void printSquare(Square * s,uint32_t colour){
	int i,j;
	Point p = s->p;
	printSquare2(p.x,p.y,s->width,s->height,colour);
}

void printSquare2(uint64_t x00,uint64_t y00,uint64_t width,uint64_t height,uint32_t colour){
	int i,j;
	for(i=x00-width/2;i<x00+width/2;i++){
		for(j=y00-height/2;j<y00+height/2;j++){
			pixel(i,j,colour);
		}
	}
}

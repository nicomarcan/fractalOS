#include <stdint.h>
#include <Square.h>
#include <Point.h>
#include <Stack.h>
#include <asmlib.h>


void printSquare(Square * s);

static Stack * squares ;
/*
 * Split:
 * 0=Horizontally
 * 1=Vertically
 */
static uint8_t mode=0;

void initializeFractal(Point * centre,uint32_t height,uint32_t width){
	squares = newStack();
	push(squares,newSquare(centre->x,centre->y,height,width));
}

void step(){
	clearscreen();
	uint32_t x,y;
	Stack * ans = newStack();
	Square * s00,* s01,* s10,* s11;
	while(!isEmpty(squares)){
		Square * s = (Square *)pop(squares);
		x=s->p.x;y=s->p.y;
		s00 = newSquare(x-(s->width)/4,y-(s->height)/4,
					      (s->width)/2,(s->height)/4);
		s01 = newSquare(x+(s->width)/4,y-(s->height)/4,
					      (s->width)/2,(s->height)/4);
		s10 = newSquare(x-(s->width)/4,y+(s->height)/4,
					      (s->width)/2,(s->height)/4);	
		s11 = newSquare(x+(s->width)/4,y+(s->height)/4,
					      (s->width)/2,(s->height)/4);
		if(mode){
			s00->p.x-=(s->width)/4;
			s01->p.x-=(s->width)/4;
			s10->p.x+=(s->width)/4;
			s11->p.x+=(s->width)/4;
		} else {
			s00->p.x-=(s->height)/4;
			s01->p.x-=(s->height)/4;
			s10->p.x+=(s->height)/4;
			s11->p.x+=(s->height)/4;
		}
		printSquare(s00);
		printSquare(s01);
		printSquare(s10);
		printSquare(s11);
		push(ans,(void *)s00);
		push(ans,(void *)s01);
		push(ans,(void *)s10);
		push(ans,(void *)s11);
	}
	mode=!mode;
}

void printSquare(Square * s){
	int i,j;
	Point * p = topLeftCorner(s);
	for(i=p->x;i<s->width;i++){
		for(j=p->y;j<s->height;j++){
			pixel(i,j,0x00FF0000);
		}
	}
}

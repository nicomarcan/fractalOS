#include <stdint.h>
#include <Square.h>
#include <Point.h>
#include <Stack.h>
#include <c_syscall.h>
#include <clib.h>
#define RED 	0x00000AFF
#define GREEN 	0x0000FFA0

static void printSquare(Square * s);

static Stack * squares ;
static uint8_t mode=0;
static uint32_t colour=RED;

void initializeFractal(Square * s){
	squares = newStack();
	push(squares,s);
	clear_screen(); 
	printSquare(s);
}

void step(){
	uint32_t x,y;
	Square * s;
	Square * s00,* s01,* s10,* s11;
	Stack * ans = newStack();
	clear_screen();
	while(!isEmpty(squares)){
		s = (Square *)pop(squares);
		x=s->p.x;y=s->p.y;
		s00 = newSquare(x-(s->width)/4,y-(s->height)/4,
					      (s->width)/2,(s->height)/2);
		s01 = newSquare(x+(s->width)/4,y-(s->height)/4,
					      (s->width)/2,(s->height)/2);
		s10 = newSquare(x-(s->width)/4,y+(s->height)/4,
					      (s->width)/2,(s->height)/2);	
		s11 = newSquare(x+(s->width)/4,y+(s->height)/4,
					      (s->width)/2,(s->height)/2);
		if(mode){
			s00->p.x-=(s->width)/4;
			s01->p.x-=(s->width)/4;
			s10->p.x+=(s->width)/4;
			s11->p.x+=(s->width)/4;
			colour=RED;
		} else {
			s00->p.y+=(s->height)/4;
			s10->p.y+=(s->height)/4;
			s01->p.y-=(s->height)/4;
			s11->p.y-=(s->height)/4;
			colour=GREEN;
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
	squares=ans;
}

static void printSquare(Square * s){
	int i,j;
	Point p = s->p;
	for(i=p.x-s->width/2;i<p.x+s->width/2;i++){
		for(j=p.y-s->height/2;j<p.y+s->height/2;j++){
			pixel(i,j,colour);
		}
	}
}

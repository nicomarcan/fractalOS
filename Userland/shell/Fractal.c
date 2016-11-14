#include <stdint.h>
#include <Square.h>
#include <Point.h>
#include <Stack.h>
#include <c_syscall.h>
#include <clib.h>
#include <libgph.h>
#include <fractal.h>

void initializeFractal(Square * s,status * st){
	st->squares = newStack();
	push(st->squares,s);
	clear_screen(); 
	printSquare(s,st->colour);
}

Stack * step(status * st){
	uint32_t x,y;
	Square * s;
	Square * s00,* s01,* s10,* s11;
	Stack * ans = newStack();
	clear_screen();
	while(!isEmpty(st->squares)){
		s = (Square *)pop(st->squares);
		x=s->p.x;y=s->p.y;
		s00 = newSquare(x-(s->width)/4,y-(s->height)/4,
					      (s->width)/2,(s->height)/2);
		s01 = newSquare(x+(s->width)/4,y-(s->height)/4,
					      (s->width)/2,(s->height)/2);
		s10 = newSquare(x-(s->width)/4,y+(s->height)/4,
					      (s->width)/2,(s->height)/2);	
		s11 = newSquare(x+(s->width)/4,y+(s->height)/4,
					      (s->width)/2,(s->height)/2);
		if(st->mode){
			s00->p.x-=(s->width)/4;
			s01->p.x-=(s->width)/4;
			s10->p.x+=(s->width)/4;
			s11->p.x+=(s->width)/4;
			st->colour=RED;
		} else {
			s00->p.y+=(s->height)/4;
			s10->p.y+=(s->height)/4;
			s01->p.y-=(s->height)/4;
			s11->p.y-=(s->height)/4;
			st->colour=GREEN;
		}
		printSquare(s00,st->colour);
		printSquare(s01,st->colour);
		printSquare(s10,st->colour);
		printSquare(s11,st->colour);
		push(ans,(void *)s00);
		push(ans,(void *)s01);
		push(ans,(void *)s10);
		push(ans,(void *)s11);
		deleteSquare(s);
	}
	st->mode=!st->mode;
	st->squares=ans;
	return ans;
}

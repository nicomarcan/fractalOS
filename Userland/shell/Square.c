#include <Square.h>
#include <Point.h>
#include <lib.h>

Square * newSquare(uint32_t x,uint32_t y,uint32_t height,uint32_t width){
  Square * ans = malloc(sizeof(Square));
  ans->p=*newPoint(x,y);
  ans->height=height;
  ans->width=width;
  return ans;
}

Point * topLeftCorner(Square * s){
  uint32_t p,q;
  p=s->p.x-(s->width)/2;
  q=s->p.y-(s->height)/2;
  return newPoint(p,q);
}

void deleteSquare(Square * s){
	deletePoint(&(s->p));
	free(s);
}

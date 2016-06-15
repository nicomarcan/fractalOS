#include <Square.h>
#include <Point.h>

Square * newSquare(Point p,uint32_t height,uint32_t width){
  Square * ans = malloc(sizeof(Square));
  ans->p=p;
  ans->height=height;
  ans->width=width;
  return ans;
}

Point * topLeftCorner(Square * s){
  uint32_t x,y;
  x=x(s->p)-(s->width)/2;
  y=y(s->p)-(s->height)/2;
  return newPoint(x,y);
}

#include <Point.h>
#include <lib.h>

Point * newPoint(uint32_t x,uint32_t y){
  Point * ans = malloc(sizeof(Point));
  ans->x=x;
  ans->y=y;
  return ans;
}

void deletePoint(Point * p){
	free(p);
}

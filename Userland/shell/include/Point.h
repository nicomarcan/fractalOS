#ifndef POINT_H
#define POINT_H
#include <stdint.h>

typedef struct Point{
  uint32_t x,y;
} Point ;

Point * newPoint(uint32_t x,uint32_t y);
void deletePoint(Point * p);

#endif

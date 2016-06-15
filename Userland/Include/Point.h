#ifndef POINT_H
#define POINT_H
#include <stdint.h>

typedef struct {
  uint32_t x,y;
} Point ;

Point * newPoint(uint32_t x,uint32_t y);
uint32_t x(Point * p);
uint32_t y(Point * p);

#endif

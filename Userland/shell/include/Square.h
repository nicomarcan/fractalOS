#ifndef SQUARE_H
#define SQUARE_H
#include <stdint.h>
#include <Point.h>
typedef struct Square {
  Point p;
  uint32_t height,width;
} Square ;

Square * newSquare(uint32_t x,uint32_t y,uint32_t height,uint32_t width);
Point * topLeftCorner(Square * s);
void deleteSquare(Square *s);

#endif

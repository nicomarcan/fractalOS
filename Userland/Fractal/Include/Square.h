#ifndef SQUARE_H
#define SQUARE_H
#include <stdint.h>

typedef struct {
  Point p;
  uint32_t height,width;
} Square ;

Square * newSquare(Point p,uint32_t height,uint32_t width);
Point * topLeftCorner(Square * s);

#endif

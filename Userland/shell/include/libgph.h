#ifndef LIBGPH_H
#define LIBGPH_H
#include <Square.h>
#include <stdint.h>
#define CENTRE_X 384
#define CENTRE_Y 512
#define RED 	0x00000AFF
#define GREEN 	0x0000FFA0
#define BLUE    0x00FFA000


typedef void (*circleprinter)(uint64_t,uint64_t,uint64_t,uint32_t);

void printSquare(Square * s,uint32_t colour);
void printSquare2(uint64_t x00,uint64_t y00,uint64_t width,uint64_t height,uint32_t colour);
void printCircle(uint64_t x0, uint64_t y0, uint64_t radius,uint32_t colour);
void printCircleFilled(uint64_t x0, uint64_t y0, uint64_t radius,uint32_t colour);
void printCircleFilled2(uint64_t x0, uint64_t y0, uint64_t radius,uint32_t colour);
void printLine(uint64_t x1, uint64_t y1, uint64_t x2, uint64_t y2, uint32_t colour);

#endif

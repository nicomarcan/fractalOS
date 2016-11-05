#ifndef LIBGPH_H
#define LIBGPH_H
#include <Square.h>
#include <stdint.h>


#define CENTRE_X 384
#define CENTRE_Y 512
#define RED 	0x00000AFF
#define GREEN 	0x0000FFA0
#define BLUE    0x00FFA000

void printSquare(Square * s,uint32_t colour);
void printSquare2(uint64_t x00,uint64_t y00,uint64_t width,uint64_t height,uint32_t colour);

#endif

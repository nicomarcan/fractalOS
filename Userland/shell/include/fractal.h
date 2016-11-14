#ifndef FRACTAL_H
#define FRACTAL_H
#include <Stack.h>
typedef struct status{
	Stack * squares;
	uint8_t mode;
	uint32_t colour;
}status;

int fractalMain(uint64_t argc, uint8_t** argv);

#endif

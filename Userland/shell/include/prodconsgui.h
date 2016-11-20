#ifndef PRODCONSGUI_H
#define PRODCONSGUI_H
#include <stdint.h>
#include <c_syscall.h>
typedef struct guiprodcon{
	int next;
    int max;
	double cosang[30];
	double sinang[30];
    mutex * m;
    uint64_t r;
} guiprodcon;

guiprodcon * renderinit(uint64_t max_size, uint64_t radius);
void renderdestroy(guiprodcon * gpc);
void renderenque(guiprodcon * gs);
void renderdeque(guiprodcon * gs);

#endif

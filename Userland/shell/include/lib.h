#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void bcopy(const void *src, void *dest, uint64_t n);
void * memcpy(void * destination, const void * source, uint64_t length);
void * malloc(uint32_t size);
void * realloc(void * ptr,uint64_t size);
void free(void * ptr);

#endif

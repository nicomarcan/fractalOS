#ifndef CLIB_H
#define CLIB_H
#include <stdint.h>
#define BUFSIZ 0XFF

void putchar(uint8_t c);
void putchars(uint8_t * str, uint64_t c);
int64_t getchar(void);
int64_t c_strcmp(const uint8_t * a, const uint8_t * b);
void print_time(uint8_t * buf);
int64_t rand();
void srand (uint64_t seed);
void clear_screen();
#endif

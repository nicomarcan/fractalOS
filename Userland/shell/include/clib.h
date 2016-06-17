#ifndef CLIB_H
#define CLIB_H
#define BUFSIZ 0XFF

void putchar(uint8_t c);
void putchars(uint8_t * str, uint64_t c);
int64_t getchar(void);
int64_t c_strcmp(const uint8_t * a, const uint8_t * b);
int64_t get_time(uint8_t * buf);
#endif

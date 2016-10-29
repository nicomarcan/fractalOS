#ifndef C_SYSCALL_H
#define C_SYSCALL_H
#include <stdint.h>
#define STDIN  0
#define STDOUT 1
#define STDERR 2

typedef struct{
	uint8_t sec,min,hour,day,month;
	uint32_t year;
} TIME;

int64_t write(uint64_t fd,const uint8_t * buf, uint64_t count );
int64_t read(uint64_t fd, const uint8_t * buf, uint8_t count);
void fkexec(void * ptr);
void exit();
TIME * time();
void sleep(uint64_t ticks);
void clear();
void set_color(uint8_t r, uint8_t g, uint8_t b);
void set_back_color(uint8_t r, uint8_t g, uint8_t b);
void pixel(uint32_t x,uint32_t y,uint32_t ccoord);
#endif

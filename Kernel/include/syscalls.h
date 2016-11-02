#ifndef SYS_SYSCALLS_H
#define SYS_SYSCALLS_H
#include <stdint.h>
#define STDIN  0
#define STDOUT 1
#define STDERR 2

int64_t sys_read(uint64_t fd,uint8_t *buf,uint64_t count);
int64_t sys_write(uint64_t fd,const uint8_t *buf, uint64_t count);

void sys_pixel(uint32_t x,uint32_t y,uint32_t ccoord);
void * sys_mem(uint64_t size);
void sys_free_mem(void * p);
void sys_sleep(uint64_t ticks);
void sys_clearscreen();
void sys_set_color(uint8_t r, uint8_t g, uint8_t b);
void sys_set_back_color(uint8_t r, uint8_t g, uint8_t b);
void sys_kill(uint64_t pid,uint64_t mode);
#endif

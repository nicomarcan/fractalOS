#ifndef SYS_SYSCALLS_H
#define SYS_SYSCALLS_H
#include <stdint.h>
#define STDIN  0
#define STDOUT 1
#define STDERR 2
int sys_read(int fd,char *buf,int count);
void sys_write(uint64_t fd,const uint8_t *buf, uint64_t count);
int sys_read(int fd,char *buf,int count);
void sys_pixel(uint32_t x,uint32_t y,uint32_t ccoord);
void * sys_mem(uint32_t size);
void sys_sleep(uint64_t ticks);
void sys_clearscreen();
#endif

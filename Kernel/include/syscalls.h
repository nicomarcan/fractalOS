#ifndef SYSCALLS_H
#define SYSCALLS_H

#define STDOUT 1
#define STDERR 2

void sys_write(int fd,const void *buf, int count);

#endif

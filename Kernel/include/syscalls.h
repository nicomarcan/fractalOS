#ifndef SYSCALLS_H
#define SYSCALLS_H

#define STDIN  0
#define STDOUT 1
#define STDERR 2

void sys_read(int fd,char *buf,int count);
void sys_write(int fd,const char *buf, int count);

#endif

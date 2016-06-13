#ifndef SYSCALLS_H
#define SYSCALLS_H

#define STDIN  0
#define STDOUT 1
#define STDERR 2

int sys_read(int fd,char *buf,int count);
void sys_write(unsigned int fd,const char *buf, unsigned int count);

#endif

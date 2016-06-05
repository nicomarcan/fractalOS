#include <syscalls.h>

/*
 * EAX:4
 */ 
void sys_write(int fd,const void *buf, int count){
	switch(fd){
		case STDOUT:
			break;
		case STDERR:
			break;
	}
	return ;
}

#include <syscalls.h>
#include <kb_driver.h>

/*
 * EAX:4
 */ 
void sys_write(int fd,const char *buf, int count){
	switch(fd){
		case STDOUT:
			break;
		case STDERR:
			break;
	}
	return ;
}

void sys_read(int fd,char *buf,int count){
	int i;
	unsigned char c;
	switch(fd){
		case STDIN:
			while((c=getChar())!=-1 && i<count){
				buf[i]=c;
				i++;
			}
			break;
		default:
			break;
	}
	return ;
}

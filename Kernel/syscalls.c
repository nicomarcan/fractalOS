#include <syscalls.h>
#include <kb_driver.h>

#define SYS_OUT_COLOR			0x29
#define SYS_ERR_COLOR			0x49

static void sys_write_out(const void * buf, unsigned int count);
static void sys_write_err(const void * buf, unsigned int count);

static char * vptr = (char *) 0xB8000;
/*
 * EAX:4
 */
void sys_write(unsigned int fd,const char *buf, unsigned int count){
	
	switch(fd){
		case STDOUT:
			sys_write_out(buf, count);
			break;
		case STDERR:
			sys_write_out(buf, count);
			break;
	}
	return ;
}
/* stdout */
static void sys_write_out(const void * buf, unsigned int count){

	print(buf, &vptr, SYS_OUT_COLOR, count);
}

/* std_err */
static void sys_write_err(const void * buf, unsigned int count){
	print(buf, &vptr, SYS_ERR_COLOR, count);
}

int sys_read(int fd,char *buf,int count){
	int i = 0;
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
	return i;
}

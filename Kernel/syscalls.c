#include <syscalls.h>
#include <kb_driver.h>
#include <video_driver.h>
#include <naiveConsole.h>
#define SYS_OUT_COLOR			0x29
#define SYS_ERR_COLOR			0x49

static void sys_write_out(const uint8_t * buf, uint64_t count);
static void sys_write_err(const uint8_t * buf, uint64_t count);

static char * vptr = (char *) 0xB8000;
/*
 * EAX:4
 */
void sys_write(uint64_t fd,const uint8_t *buf, uint64_t count){

	switch(fd){
		case STDOUT:
			sys_write_out(buf, count);
			break;
		case STDERR:
			sys_write_err(buf, count);
			break;
	}
	return ;
}
/* stdout */
static void sys_write_out(const uint8_t * buf, uint64_t count){

	print(buf, count);
}

/* std_err */
static void sys_write_err(const uint8_t * buf, uint64_t count){
	print(buf, count);
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

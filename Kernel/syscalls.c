#include <syscalls.h>
#include <fifo.h>
#include <kb_driver.h>
#include <video_driver.h>
#include <asmlib.h>
#include <lib.h>
#include <rtc.h>
#include <liballoc.h>
#include <Scheduler.h>
#include <Process.h>
#define SYS_OUT_COLOR			0x29
#define SYS_ERR_COLOR			0x49

extern Process * foreground;
static int64_t sys_write_out(const uint8_t * buf, uint64_t count);
static int64_t sys_write_err(const uint8_t * buf, uint64_t count);

/*
 * EAX:4
 */
int64_t sys_write(uint64_t fd,const uint8_t *buf, uint64_t count){
	int64_t ret;
	switch(fd){
		case STDOUT:
			ret = sys_write_out(buf, count);
			break;
		case STDERR:
			ret = sys_write_err(buf, count);
			break;
		default:
			ret = write_fifo(fd,buf,count);
			return;
	}
	return ret;
}
/* stdout */
static int64_t sys_write_out(const uint8_t * buf, uint64_t count){

	print(buf, count);
}

/* std_err */
static int64_t sys_write_err(const uint8_t * buf, uint64_t count){
	print(buf, count);
}

int64_t sys_read(uint64_t fd,uint8_t *buf,uint64_t count){

	int64_t i = 0;
	int8_t c;
	switch(fd){
		case STDIN:
			while(!isFg()){
				if(currPid()==1){
					char buff[10];
					intToString(buff,foreground->pid);
					print(buff,1);
				}
				yield();
			}
			while(i<count && (c=getChar())!=-1){
				if (c > 0) {
					buf[i]=c;
					i++;
				}
			}
			break;
		default:
			i = read_fifo(fd,buf,count);
			break;
	}
	return i;
}

void sys_ipcs(OPENED_FIFOS * of){
	 get_opened_fifos(of);
}

int64_t sys_mkfifo(const char * addr){
	return mkfifo(addr);
}

int64_t sys_rmfifo(const char * addr){
	return rmfifo(addr);
}
int64_t sys_write_fifo(uint64_t fd,const uint8_t * buf, uint64_t count ){
	return write_fifo(fd,buf,count);
}

int64_t sys_read_fifo(uint64_t fd, uint8_t * buf, uint64_t count ){
	return read_fifo(fd,buf,count);
}

int64_t sys_open_fifo(const char * addr,uint64_t mode){
	 return open_fifo(addr,mode);
}
int64_t sys_close_fifo(const char * addr,uint64_t mode){
	 return close_fifo(addr,mode);
}


TIME * sys_time() {
	return time();
}


void * sys_realloc(void * p,uint64_t nsize){
	return la_realloc(p,nsize);
}

void * sys_mem(uint64_t size) {
	return la_malloc(size);
}

void sys_free_mem(void * p){
	la_free(p);
	return;
}

void sys_pixel(uint32_t x,uint32_t y,uint32_t ccoord){
	print_pixel(x,y,ccoord);
}

void sys_clearscreen(){
	clear_screen();
}

void sys_set_color(uint8_t r, uint8_t g, uint8_t b) {
	set_font_color(r,g,b);
}

void sys_set_back_color(uint8_t r, uint8_t g, uint8_t b) {
	set_back_color(r,g,b);
}

void sys_kill(uint64_t pid,uint64_t mode){
	switch(mode){
		/* kill 0: eliminate a process */
		case 0:
			deleteProcessScheduler(pid);
			break;
		/* kill 1: wake a process */
		case 1:
			wake(pid);
			break;
		/* kill 2: make a process wait */
		case 2:
			mkwait(pid);
			break;
		default:
			break;
	}
	return;
}

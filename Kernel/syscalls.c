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
static mutex fifo_m;


void fifoinitlock(){
	mutex_nameinit(&fifo_m,"fifo_mainlock");
}

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
			mutex_lock(&fifo_m);
			ret = write_fifo(fd,buf,count);
			mutex_unlock(&fifo_m);
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
			mutex_lock(&fifo_m);
			i = read_fifo(fd,buf,count);
			mutex_unlock(&fifo_m);
			break;
	}
	return i;
}

void sys_infofifos(OPENED_FIFOS * of){
	 get_opened_fifos(of);
}

int64_t sys_mkfifo(const char * addr){
	mutex_lock(&fifo_m);
	int64_t ret = mkfifo(addr);
	mutex_unlock(&fifo_m);
	return ret;
}

int64_t sys_rmfifo(const char * addr){
	mutex_lock(&fifo_m);
	int64_t ret = rmfifo(addr);
	mutex_unlock(&fifo_m);
	return ret;
}
int64_t sys_write_fifo(uint64_t fd,const uint8_t * buf, uint64_t count ){
	mutex_lock(&fifo_m);
	int64_t ret = write_fifo(fd,buf,count);
	mutex_unlock(&fifo_m);
	return ret;
}

int64_t sys_read_fifo(uint64_t fd, uint8_t * buf, uint64_t count ){
	mutex_lock(&fifo_m);
	int64_t ret = read_fifo(fd,buf,count);
	mutex_unlock(&fifo_m);
	return ret;
}

int64_t sys_open_fifo(const char * addr,uint64_t mode){
	 mutex_lock(&fifo_m);
	 int64_t ret = open_fifo(addr,mode);
	 mutex_unlock(&fifo_m);
	 return ret;
}
int64_t sys_close_fifo(const char * addr,uint64_t mode){
	 mutex_lock(&fifo_m);
	 int64_t ret = close_fifo(addr,mode);
	 mutex_unlock(&fifo_m);
	 return ret;
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

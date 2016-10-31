#include <syscalls.h>
#include <kb_driver.h>
#include <video_driver.h>
#include <asmlib.h>
#include <lib.h>
#include <rtc.h>
#include <liballoc.h>
#define SYS_OUT_COLOR			0x29
#define SYS_ERR_COLOR			0x49

static int64_t sys_write_out(const uint8_t * buf, uint64_t count);
static int64_t sys_write_err(const uint8_t * buf, uint64_t count);

uint64_t counter;

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
			while(i<count && (c=getChar())!=-1){
				if (c > 0) {
					buf[i]=c;
					i++;
				}
			}
			break;
		default:
			break;
	}
	return i;
}

TIME * sys_time() {
	return time();
}


void sys_sleep(uint64_t ticks){
	if (ticks == 0) {
		_sti();
		_hlt();
		return;
	}
	counter=ticks;
	_sti();
	while(counter){
		_hlt();
	}
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

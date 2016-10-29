#include <interrupt_controller.h>
#include <stdint.h>
#include <rtc.h>
#include <kb_driver.h>
#include <lib.h>
#include <asmlib.h>
#include <syscalls.h>
#include <utils.h>
#include <video_driver.h>
#include <Scheduler.h>

void int_32();
void int_33();

char buffer[10];

	extern uint64_t counter;

void irqDispatcher(uint64_t irq){
	switch(irq) {
		case 0:
			int_32();
			break;
		case 1:
			int_33();
			break;
	}
	return;
}


/*
 * Timer tick
 * (irq 0)
 */
void int_32(){
	counter--;
}

/*
 * Keyboard
 * (irq 1)
 */
void int_33() {
	fetch();
}

/*
 * Interrupt for syscalls
 */

 int64_t syscall_dispatcher(uint64_t rax, uint64_t rdi, uint64_t rsi, uint64_t rdx){
	int64_t ret = 0;
 	switch(rax){
 		case 0:
 			ret = sys_read(rdi,(uint8_t *)rsi,rdx);
 			break;
 		case 1:
 			ret = sys_write(rdi,(const uint8_t*)rsi,rdx);
 			break;
 		case 4:
			fork();
			break;
 		case 5:
			exec((void *)rdi);
			break;
 		case 6:
			sys_realloc((void *)rdi,rsi);
			break;
 		case 7:
			sys_free_mem((void *)rdi);
			break;
 		case 8:
			return sys_mem(rdi);
			break;
 		case 9:
			sys_sleep(rdi);
			break;
 		case 10:
			/*
			 * rdi: x coord
			 * rsi: y coord
			 * rdx: colour coord
			 */
			sys_pixel((uint32_t) rdi,(uint32_t)rsi,(uint32_t)rdx);
			break;
		case 11:
			sys_clearscreen();
			break;
		case 12:
			ret = (int64_t) sys_time();
			break;
		case 13:
			sys_set_color(rdi, rsi, rdx);
			break;
		case 14:
			sys_set_back_color(rdi, rsi, rdx);
			break;
 		default:
 			break;
 	}
 	return ret;
 }

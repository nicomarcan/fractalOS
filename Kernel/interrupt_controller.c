#include <interrupt_controller.h>
#include <stdint.h>
#include <naiveConsole.h>
#include <rtc.h>
#include <kb_driver.h>
#include <lib.h>
#include <asmlib.h>
#include <syscalls.h>
#include <utils.h>
#include <video_driver.h>

void int_32();
void int_33();

char buffer[10];

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
	pulse();
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
 		default:
 			break;
 	}
 	return ret;
 }

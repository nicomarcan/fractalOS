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
#include <Mutex.h>


typedef int64_t (*syscall_proto)(uint64_t,uint64_t,uint64_t);


void int_32();
void int_33();

char buffer[10];


void irqDispatcher(uint64_t irq){
	switch(irq) {
		case 1:
			int_33();
			break;
		default:
			break;
	}
	return;
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
	uint64_t count = 24;
	syscall_proto syscalls[] = {
		(syscall_proto)sys_read,
		(syscall_proto)sys_write,
		(syscall_proto)ps,
		(syscall_proto)wait,
		(syscall_proto)exit,
		(syscall_proto)fkexec,
		(syscall_proto)sys_realloc,
		(syscall_proto)sys_free_mem,
		(syscall_proto)sys_mem,
		(syscall_proto)sys_sleep,
		(syscall_proto)sys_pixel,
		(syscall_proto)sys_clearscreen,
		(syscall_proto)sys_time,
		(syscall_proto)sys_set_color,
		(syscall_proto)sys_set_back_color,
		(syscall_proto)sys_kill,
		(syscall_proto)mutex_lock,
		(syscall_proto)mutex_unlock,
		(syscall_proto)currPid,
		(syscall_proto)currPpid,
		(syscall_proto)sys_mkfifo,
		(syscall_proto)sys_rmfifo,
		(syscall_proto)sys_write_fifo,
		(syscall_proto)sys_read_fifo
	};
	if(rax>=0 && rax<count){
		ret = syscalls[rax](rdi,rsi,rdx);
	}
 	return ret;
 }

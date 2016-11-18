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

	uint64_t count = 34;

	syscall_proto syscalls[] = {
		/* 00 */(syscall_proto)sys_read,
		/* 01 */(syscall_proto)sys_write,
		/* 02 */(syscall_proto)ps,
		/* 03 */(syscall_proto)fkexec,
		/* 04 */(syscall_proto)wait,
		/* 05 */(syscall_proto)exit,
		/* 06 */(syscall_proto)sys_kill,
		/* 07 */(syscall_proto)currPid,
		/* 08 */(syscall_proto)currPpid,
		/* 09 */(syscall_proto)giveFg,
		/* 10 */(syscall_proto)sys_realloc,
		/* 11 */(syscall_proto)sys_free_mem,
		/* 12 */(syscall_proto)sys_mem,
		/* 13 */(syscall_proto)sys_sleep,
		/* 14 */(syscall_proto)sys_time,
		/* 15 */(syscall_proto)sys_pixel,
		/* 16 */(syscall_proto)sys_clearscreen,
		/* 17 */(syscall_proto)sys_set_color,
		/* 18 */(syscall_proto)sys_set_back_color,
		/* 19 */(syscall_proto)mutex_init,
		/* 20 */(syscall_proto)mutex_destroy,
		/* 21 */(syscall_proto)mutex_lock,
		/* 22 */(syscall_proto)mutex_unlock,
		/* 23 */(syscall_proto)mutex_nameinit,
		/* 24 */(syscall_proto)mutex_getbyname,
		/* 25 */(syscall_proto)release_lock_and_sleep,
		/* 26 */(syscall_proto)mutex_info,
		/* 27 */(syscall_proto)sys_infofifos,
		/* 28 */(syscall_proto)sys_mkfifo,
		/* 29 */(syscall_proto)sys_rmfifo,
		/* 30 */(syscall_proto)sys_write_fifo,
		/* 31 */(syscall_proto)sys_read_fifo,
		/* 32 */(syscall_proto)sys_open_fifo,
		/* 33 */(syscall_proto)sys_close_fifo
	};
	if(rax>=0 && rax<count){
		ret = syscalls[rax](rdi,rsi,rdx);
	}
 	return ret;
 }

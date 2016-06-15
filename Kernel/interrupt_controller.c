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

}

/*
 * Keyboard
 * (irq 1)
 */
void int_33(){
	unsigned char c;
	char * buff;
	if(fetch()){
		c=peekChar();
		if(c!=0){
			if(c=='\n'){
				//ncNewline();
				print_newline();
			} else if (c=='\b'){
				buff = (char*)malloc(10);
				//ncPrint(timeStr(buff));
				print_string(timeStr(buff));
			} else {
				//ncPrintChar(c);
				/*
				 * to use with graphical mode
				 */
				print_char((uint8_t)c);
			}
		}
	}
}

/*
 * Interrupt for syscalls
 */
 void syscall_dispatcher(uint64_t rax, uint64_t rdi, uint64_t rsi, uint64_t rdx){

 	switch(rax){
 		case 0:
 			sys_read(rdi,(char *)rsi,rdx);
 			break;
 		case 1:
 			sys_write((unsigned int) rdi,(const char*)rsi, (unsigned int)rdx);
 			break;
 		default:
 			break;
 	}

 	return ;
 }

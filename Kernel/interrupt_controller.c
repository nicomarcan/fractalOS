#include <stdint.h>
#include <naiveConsole.h>
#include <rtc.h>
#include <kb_driver.h>

char buffer[100]={0};

void int_32();
void int_33();

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
//	static int i ;
//	char *video = (char *) 0xb8000;
//	video[i++] = (char) i;
}

/*
 * Keyboard
 * (irq 1)
 */
void int_33(){
	char c;
	fetch();
	c=getChar();
	if(c!=0){
		if(c=='\n'){
			ncNewline();
		} else if (c=='\b'){
			ncPrint(timeStr(buffer));
		} else {	
			ncPrintChar(c);
		}
	}
}

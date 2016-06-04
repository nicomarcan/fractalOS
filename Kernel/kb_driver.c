#include <lib.h>
#define BUFF_SIZE 0xFF

static char kb_getc();

unsigned char kbdus[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
    ' ',/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};	


typedef struct{
	char array[BUFF_SIZE];
	int i;
	int j;
	int not_read;
} CIRC_BUFFER;

CIRC_BUFFER buff={{0},0,0,0};

/*
 * Fetches a char from the keyboard.
 * Use only in int_33()
 */
char fetch(){
	char c=kb_getc();
	buff.array[buff.i]=c;
	buff.i++;
	if(buff.i==BUFF_SIZE) buff.i=0;
	buff.not_read++;
	return c;
}

/*
 * Returns a char from the buffer.
 * -1 if no chars to read
 */
char getChar(){
	if(buff.not_read==0) return -1;
	char ans = buff.array[buff.j];
	buff.j++;
	if(buff.j==BUFF_SIZE) buff.j=0; 
	buff.not_read--;
	return ans;
}

/*
 * Decreases the read pointer by one and
 * restores the given char.
 */
void ungetc(char c){
	if(buff.j==0){
		buff.j=BUFF_SIZE-1;
	}else{
		buff.j--;
	}
	buff.not_read++;
	buff.array[buff.j]=c;
}	

static char kb_getc(){
	char c=(char)kb_read();
	if (c>=0x80) return 0;
	return kbdus[c];
}

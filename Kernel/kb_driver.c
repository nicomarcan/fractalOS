#include <asmlib.h>
#include <kb_layout.h>

#define BUFF_SIZE 0xFF

static char shift=0;

static char kb_getc();

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
 * Peeks the last char inserted
 * to the buffer.
 * -1 if no chars inserted
 */

char peekChar(){
	if(buff.not_read==0) return -1;
	if(buff.i==0){
		return buff.array[BUFF_SIZE-1];
	}
	return buff.array[buff.i-1];
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
	switch(c){
		case KRLEFT_SHIFT:
			shift=1;
			break;
		case KRLEFT_SHIFT_BK:
			shift=0;
			break;
		default:
			if(c>0x80) return 0;
			break;
	}
	if (shift){ 
		return asciiShift[c];
	}
	return asciiNonShift[c];
}

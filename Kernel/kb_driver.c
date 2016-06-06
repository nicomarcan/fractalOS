#include <asmlib.h>
#include <kb_layout.h>
#include <naiveConsole.h>

#define BUFF_SIZE 0xFF

static char shift=0;

static void insert(unsigned char c);

typedef struct{
	unsigned char array[BUFF_SIZE];
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
	unsigned char c=(unsigned char)kb_read();
	switch (c){
		case KRLEFT_SHIFT:
			shift=1;
			break;
		case KRLEFT_SHIFT_BK:
			shift=0;
			break;
		default:
			break;
	}
	if(c>0x80){
		return 0;
	}
	if (shift) {
		insert(asciiShift[c]);
	} else {
		insert(asciiNonShift[c]);
	}
	return 1;
}

static void insert(unsigned char c){
	buff.array[buff.i]=c;
	buff.i++;
	if(buff.i==BUFF_SIZE) buff.i=0;
	buff.not_read++;
}

/*
 * Returns a char from the buffer.
 * -1 if no chars to read
 */
unsigned char getChar(){
	if(buff.not_read==0) return -1;
	unsigned ans = buff.array[buff.j];
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
unsigned char peekChar(){
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
void ungetc(unsigned char c){
	if(buff.j==0){
		buff.j=BUFF_SIZE-1;
	}else{
		buff.j--;
	}
	buff.not_read++;
	buff.array[buff.j]=c;
}

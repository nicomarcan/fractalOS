#ifndef KB_DRIVER_H
#define KB_DRIVER_H
char fetch();
unsigned char getChar();
unsigned char peekChar();
void ungetc(char c);
#endif

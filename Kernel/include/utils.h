#ifndef UTILS_H
#define UTILS_H
#include <stdint.h>
int intToString(char * buffer,int num);
void charToString(char*buffer,char c);
int strlen(const char *s);
int64_t strcmp(const uint8_t * a, const uint8_t * b);
uint8_t * strcpy ( uint8_t * destination, const uint8_t * source );


#endif

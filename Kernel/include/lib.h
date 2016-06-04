#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);

char *cpuVendor(char *result);

uint64_t _rax();
uint64_t _rbx();
uint64_t _rcx();
uint64_t _rdx();
uint64_t _rbp();
uint64_t _rsi();
uint64_t _rdi();
uint64_t _rsp();
uint64_t _r8();
uint64_t _r9();
uint64_t _r10();
uint64_t _r11();
uint64_t _r12();
uint64_t _r13();
uint64_t _r14();
uint64_t _r15();
void _lidt(void * idtr);
void picMasterMask(uint8_t mask);
void picSlaveMask(uint8_t mask);
void _irq00handler();
void _irq01handler();
void _cli();
void _sti();
char kb_read();
int intToString(char * buffer,int num);
void charToString(char*buffer,char c);
uint64_t rtc(uint64_t mode);
#endif

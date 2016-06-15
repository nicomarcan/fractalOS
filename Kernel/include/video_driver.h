#ifndef VIDEO_DRIVER_H
#define VIDEO_DRIVER_H
#include <stdint.h>
void initialize_driver();
void naive_print();
void clear_screen();
void print_char(uint8_t character);
void print_string(char * str);
void print_newline();
void print_pixel(int p,int q,uint8_t b,uint8_t g,uint8_t r);
#endif

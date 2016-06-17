#ifndef SHELL_H
#include <stdint.h>
#define SHELL_H
#define COMM_TABLE_MAX_SIZE 100
#define MAX_ARGUMENT_SIZE 10
#define KEYBOARD_BUFF_SIZE 50

void init_shell();
uint8_t shell();
#endif

#ifndef COMMAND_H
#define COMMAND_H
#include <stdint.h>

int64_t hello_world(uint64_t argc, uint8_t * argv[]);
int64_t curr_time(uint64_t argc, uint8_t * argv[]);
int64_t echo(uint64_t argc, uint8_t * argv[]);

#endif

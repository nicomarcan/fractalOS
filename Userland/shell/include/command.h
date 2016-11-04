#ifndef COMMAND_H
#define COMMAND_H
#include <stdint.h>

int64_t hello_world(uint64_t argc, uint8_t * argv[]);
int64_t curr_time(uint64_t argc, uint8_t * argv[]);
int64_t echo(uint64_t argc, uint8_t * argv[]);
int64_t _ps(uint64_t argc, uint8_t * argv[]);
int64_t _kill(uint64_t argc, uint8_t * argv[]);
int64_t infiloop(uint64_t argc, uint8_t * argv[]);
int64_t waiter(uint64_t argc, uint8_t * argv[]);
int64_t mutextest(uint64_t argc, uint8_t * argv[]);
int64_t mkfifoc(uint64_t argc, uint8_t * argv[]);
int64_t write_fifoc(uint64_t argc, uint8_t * argv[]);
int64_t read_fifoc(uint64_t argc, uint8_t * argv[]);
#endif

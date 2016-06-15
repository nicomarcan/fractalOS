#include <stdint.h>
#define STDIN  0
#define STDOUT 1
#define STDERR 2
uint64_t write(uint64_t fd,const uint8_t * buf, uint64_t count );
uint64_t read(uint64_t fd, const uint8_t * buf, uint8_t count);

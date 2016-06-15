#include <c_syscall.h>
#include <clib.h>

void putchar(uint8_t c){
  write(STDOUT, &c, 1);
}

void putchars(uint8_t * str, uint64_t c) {
  write(STDOUT, str, c);
}

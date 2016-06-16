#ifndef INTERRUPT_CONTROLLER_H
#define INTERRUPT_CONTROLLER_H
#include <stdint.h>

void irqDispatcher(uint64_t irq);
int64_t syscall_dispatcher(uint64_t rax, uint64_t rdi, uint64_t rsi, uint64_t rdx);
void int_33();
void int_32();

#endif

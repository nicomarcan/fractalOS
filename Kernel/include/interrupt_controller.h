#ifndef INTERRUPT_CONTROLLER_H
#define INTERRUPT_CONTROLLER_H

void irqDispatcher(uint64_t irq);
void syscall_dispatcher(uint64_t rax, uint64_t rdi, uint64_t rsi, uint64_t rdx);
void int_33();
void int_32();

#endif

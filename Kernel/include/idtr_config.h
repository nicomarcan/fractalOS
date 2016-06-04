#ifndef IDTR_CONFIG_H
#define IDTR_CONFIG_H

#include <types.h>

void setup_IDT_entry(int index,uint16_t selector,uint64_t offset,uint8_t attr);
void load_IDTR();

#endif

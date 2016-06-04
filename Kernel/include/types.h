#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#pragma pack(push)
#pragma pack(1)

typedef struct{
	uint16_t limit;
	uint64_t offset;
}IDTR;

typedef struct{
	uint16_t offset_lo;
	uint16_t selector;
	uint8_t zero1;
	uint8_t type_attr;
	uint16_t offset_mi;
	uint32_t offset_hi;
	uint32_t zero2;
}DESCR_INT;

#pragma pack(pop)

#endif

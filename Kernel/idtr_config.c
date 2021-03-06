#include <types.h>
#include <asmlib.h>

/*
* Basic functions for setting up
* IDT entries and then loading them
* into the IDTR register.
*
* author: saques
*/

/*
 * Pure64 loads an IDT in the range
 * 000 - FFF.
 */
static DESCR_INT * idt = 0;
static IDTR idtr;

#define LO_16 0x000000000000FFFF
#define LO_32 0x00000000FFFFFFFF


void setup_IDT_entry(int index,uint16_t selector,uint64_t offset,uint8_t type_attr){
	idt[index].zero1=idt[index].zero2=0;
	idt[index].selector=selector;
	idt[index].type_attr=type_attr;
	idt[index].offset_lo=offset&LO_16;
	idt[index].offset_mi=(offset>>0x10)&LO_16;
	idt[index].offset_hi=(offset>>0x20)&LO_32;
}

/*
 * Using the IDT pre-loaded by Pure64,
 * this function is not necessary. Use
 * setup_IDT_entry() to load the desired
 * descriptors to the pre-loaded table.
 */
void load_IDTR(){
	idtr.limit=(uint16_t)(sizeof(idt)-1);
	idtr.offset=0;
	idtr.offset+=(uint64_t)&idt;
	_lidt(&idtr);
}




 

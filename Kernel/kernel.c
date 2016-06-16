#include <stdint.h>
#include <string.h>
#include <asmlib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <idtr_config.h>
#include <video_driver.h>
#include <clib.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;
static void * const fractalModuleAddress = (void*)0x600000;

typedef int (*EntryPoint)();
void int_08();

void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary()
{
	char buffer[10];

	/*
	 * Load modules
	 */
	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress,
		fractalModuleAddress
	};
	loadModules(&endOfKernelBinary, moduleAddresses);

	clearBSS(&bss, &endOfKernel - &bss);
	return getStackBase();
}

int main()
{

	setup_IDT_entry(32,0x8,(uint64_t)&_irq00handler,0x8E);
	setup_IDT_entry(33,0x8,(uint64_t)&_irq01handler,0x8E);
	setup_IDT_entry(0x80,0x8,(uint64_t)&_syscall_handler,0x8E);

	initialize_driver();
	ncPrintDec(20);
	c = getchar();
	putchar(c);

	((EntryPoint)fractalModuleAddress)();

	for(;;);
	return 0;
}

#include <stdint.h>
#include <string.h>
#include <asmlib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <idtr_config.h>
#include <video_driver.h>

//unreal mode: pasar a modo 16 bits, int 10 y volver
//modo grafico desde cero: consola grafica
//driver simple para escribir a puerto serial
//  qemu: todo lo que sale a puerto serial lo imprime por
//  la consola de linux
//flag de qemu que tiene una bios especial para acceder a modo
//grafico desde 64 bits bochs

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;

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

	ncPrint("[x64BareBones]");
	ncNewline();

	ncPrint("CPU Vendor:");
	ncPrint(cpuVendor(buffer));
	ncNewline();

	ncPrint("[Loading modules]");
	ncNewline();
	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);
	ncPrint("[Done]");
	ncNewline();
	ncNewline();

	ncPrint("[Initializing kernel's binary]");
	ncNewline();

	clearBSS(&bss, &endOfKernel - &bss);

	ncPrint("  text: 0x");
	ncPrintHex((uint64_t)&text);
	ncNewline();
	ncPrint("  rodata: 0x");
	ncPrintHex((uint64_t)&rodata);
	ncNewline();
	ncPrint("  data: 0x");
	ncPrintHex((uint64_t)&data);
	ncNewline();
	ncPrint("  bss: 0x");
	ncPrintHex((uint64_t)&bss);
	ncNewline();

	ncPrint("[Done]");
	ncNewline();
	ncNewline();
	return getStackBase();
}

int main()
{
	int i,j;
	ncPrint("[Kernel Main]");
	ncNewline();
	ncPrint("  Sample code module at 0x");
	ncPrintHex((uint64_t)sampleCodeModuleAddress);
	ncNewline();
	ncPrint("  Calling the sample code module returned: ");
	ncPrintHex(((EntryPoint)sampleCodeModuleAddress)());
	ncNewline();
	ncNewline();

	ncPrint("  Sample data module at 0x");
	ncPrintHex((uint64_t)sampleDataModuleAddress);
	ncNewline();
	ncPrint("  Sample data module contents: ");
	ncPrint((char*)sampleDataModuleAddress);
	ncNewline();

	ncPrint("[Finished]");


	setup_IDT_entry(32,0x8,(uint64_t)&_irq00handler,0x8E);
	setup_IDT_entry(33,0x8,(uint64_t)&_irq01handler,0x8E);
	setup_IDT_entry(0x80,0x8,(uint64_t)&_syscall_handler,0x8E);

	_cli();

	picMasterMask(0xFC);
	picSlaveMask(0xFF);

	_sti();

	initialize_driver();

	for(i=100;i<200;i++){
		for(j=100;j<200;j++){
			print_pixel(i,j,0,0,0xFF);
		}
	}

	for(;;);
	return 0;
}

OUTPUT_FORMAT("binary")
ENTRY(loader)
SECTIONS
{
	.text 0x700000 :
	{
		*(.text*)
		. = ALIGN(0x1000);
		*(.rodata*)
	}
	.data ALIGN(0x1000) :
	{
		*(.data*)
	}
	.bss ALIGN(0x1000) :
	{
		bss = .;
		*(.bss*)
	}
	endOfBinary = .;
}

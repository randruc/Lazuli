/*
 * Architecure independent linker script.
 */

INCLUDE kern/arch/AVR/linker.x

ENTRY(interrupt_vectors_table)

SECTIONS
{
	. = 0x0;
	
	.text :
	{
		*(.text)
		_text_end = .;
	} > REGION_TEXT
	
	.data : AT(_text_end)
	{
		_data_start = .;
		*(.data)
	} > REGION_DATA
	_data_size = SIZEOF(.data);
	_data_load_start = LOADADDR(.data);
	
	.bss :
	{
		_bss_start = .;
		*(.bss)
	} > REGION_BSS
	_bss_size = SIZEOF(.bss);
	_brk = .;
	_ramend = LENGTH(ram) - 1;
}

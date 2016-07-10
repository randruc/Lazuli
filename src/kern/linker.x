/*
 * Linker script
 */

/*
 * These memory regions are specific to the atmega328p.
 *
 * TODO: Separate this in an arch-specific file
 */
MEMORY
{
	rom : ORIGIN = 0x0, LENGTH = 16K
	
	/* 
	 * The origin must be offset by 0x800000 on the
	 * harvard architecure of the atmega328p
	 */
	ram : ORIGIN = 0x800000 + 0x100, LENGTH = 2K
}

REGION_ALIAS("REGION_TEXT", rom);
REGION_ALIAS("REGION_DATA", ram);
REGION_ALIAS("REGION_BSS", ram);

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
}

/*
 * Linker script.
 * Specific to the atmega328p.
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

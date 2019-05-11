/*
 * Linker script.
 * Specific to the atmega328p.
 */

MEMORY
{
    rom : ORIGIN = 0x0, LENGTH = 32K

    /*
     * The origin of RAM must be offset by 0x800000 because that's how ld deals
     * with RAM and ROM memories in Harvard architecture configuration.
     * We add 0x100 to skip memory-mapped registers.
     */
    ram : ORIGIN = 0x800000 + 0x100, LENGTH = 2K + 0x100
}

REGION_ALIAS("REGION_TEXT",    rom);
REGION_ALIAS("REGION_PROGMEM", rom);

REGION_ALIAS("REGION_DATA",    ram);
REGION_ALIAS("REGION_RODATA",  ram);
REGION_ALIAS("REGION_BSS",     ram);
REGION_ALIAS("REGION_NOINIT",  ram);

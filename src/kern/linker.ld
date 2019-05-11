/*
 * Architecture independent linker script.
 */

INCLUDE kern/arch/AVR/linker.ld

ENTRY(interrupt_vectors_table)

SECTIONS
{
    .text :
    {
        KEEP(*(.ivt))
        *(.text)
        *(.text.*)
    } > REGION_TEXT

    .progmem : AT(LOADADDR(.text) + SIZEOF(.text))
    {
        KEEP(*(.progmem*))
    } > REGION_PROGMEM
    _progmem_size = SIZEOF(.progmem);
    _progmem_load_start = LOADADDR(.progmem);

    .data : AT(LOADADDR(.progmem) + SIZEOF(.progmem))
    {
        KEEP(*(.data))
    } > REGION_DATA
    _data_start = ADDR(.data);
    _data_load_start = LOADADDR(.data);
    _data_size = SIZEOF(.data);

    .rodata : AT(LOADADDR(.data) + SIZEOF(.data))
    {
        /* Read-only data first */
        KEEP(*(.rodata))
        /* Then constant strings */
        KEEP(*(.rodata.str*))
    } > REGION_RODATA
    _rodata_start = ADDR(.rodata);
    _rodata_load_start = LOADADDR(.rodata);
    _rodata_size = SIZEOF(.rodata);

    .bss :
    {
        *(.bss)
    } > REGION_BSS
    _bss_start = ADDR(.bss);
    _bss_size = SIZEOF(.bss);

    .noinit :
    {
        *(.noinit)
    } > REGION_NOINIT

    _brk = .;
    _ramend = LENGTH(ram) - 1;
}

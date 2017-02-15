/**
 * @file linker.h
 *
 * Symbols defined by the linker.
 */

#ifndef LZ_LINKER_H
#define LZ_LINKER_H

#include <Lazuli/common.h>

/**
 * Start address of the .data section.
 */
extern u8 _data_start;

/**
 * Start address of the loaded .data section.
 */
extern u8 _data_load_start;

/**
 * Start address of the .rodata section.
 */
extern u8 _rodata_start;

/**
 * Start address of the loaded .rodata section.
 */
extern u8 _rodata_load_start;

/**
 * Start address of the .bss section.
 */
extern u8 _bss_start;

/**
 * Initial break address.
 */
extern u8 _brk;

/**
 * Last address of RAM.
 */
extern u8 _ramend;

#endif /* LZ_LINKER_H */

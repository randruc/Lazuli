/**
 * @file src/include/Lazuli/sys/linker.h
 * @brief Symbols defined by the linker.
 * @date Feb 2017
 * @author Remi Andruccioli
 *
 * This file contains all the symbols defined by the linker.
 * They only consist of addresses and are always accessed with the & operator.
 */

#ifndef LZ_SYS_LINKER_H
#define LZ_SYS_LINKER_H

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

/**
 * Start address of the loaded .progmem section.
 */
extern u8 _progmem_load_start;

#endif /* LZ_SYS_LINKER_H */

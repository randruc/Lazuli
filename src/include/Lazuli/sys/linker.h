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
extern void * const _data_start;

/**
 * Start address of the loaded .data section.
 */
extern void *const _data_load_start;

/**
 * Total size of the .data section.
 */
extern const u8 _data_size;

/**
 * Start address of the .bss section.
 */
extern void * const _bss_start;

/**
 * Total size of the .bss section.
 */
extern const u8 _bss_size;

/**
 * Initial break address.
 */
extern void * const _brk;

/**
 * Last address of RAM.
 */
extern void * const _ramend;

#endif /* LZ_LINKER_H */

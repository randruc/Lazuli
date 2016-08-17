/**
 * Symbols defined by the linker
 */

#ifndef LZ_LINKER_H
#define LZ_LINKER_H

/** 
 * Start address of the .data section
 */
extern void *_data_start;

/** 
 * Start address of the loaded .data section
 */
extern void *_data_load_start;

/**
 * Total size of the .data section
 */
extern unsigned char _data_size;

/**
 * Start address of the .bss section
 */
extern void *_bss_start;

/** 
 * Total size of the .bss section
 */
extern const unsigned char _bss_size;

/**
 * Initial break address.
 */
extern void *_brk;

/**
 * Last address of RAM.
 */
extern void *_ramend;

#endif /* LZ_LINKER_H */

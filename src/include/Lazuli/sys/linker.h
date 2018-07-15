/**
 * @file src/include/Lazuli/sys/linker.h
 * @brief Symbols defined by the linker.
 * @date Feb 2017
 * @author Remi Andruccioli
 *
 * This file contains all the symbols defined by the linker.
 * They only consist of addresses and are always accessed with the & operator.
 */

#ifndef LAZULI_SYS_LINKER_H
#define LAZULI_SYS_LINKER_H

#include <stdint.h>

#include <Lazuli/common.h>

/**
 * Start address of the .data section.
 */
extern uint8_t _data_start;

/**
 * Start address of the loaded .data section.
 */
extern uint8_t _data_load_start;

/**
 * Start address of the .rodata section.
 */
extern uint8_t _rodata_start;

/**
 * Start address of the loaded .rodata section.
 */
extern uint8_t _rodata_load_start;

/**
 * Start address of the .bss section.
 */
extern uint8_t _bss_start;

/**
 * Initial break address.
 */
extern uint8_t _brk;

/**
 * Last address of RAM.
 */
extern uint8_t _ramend;

/**
 * Start address of the loaded .progmem section.
 */
extern uint8_t _progmem_load_start;

#endif /* LAZULI_SYS_LINKER_H */

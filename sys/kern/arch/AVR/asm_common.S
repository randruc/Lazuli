/*
 * SPDX-License-Identifier: GPL-3.0-only
 * This file is part of Lazuli.
 */

/**
 * @file
 * @brief Common ASM macros and definitions.
 * @copyright 2019-2020, Remi Andruccioli <remi.andruccioli@gmail.com>
 *
 * This file defines commonly used AVR assembly macros and symbols in order to
 * be used in other ASM files with the .USING directive.
 */

#include <Lazuli/common_asm.h>
#include <Lazuli/sys/kernel_asm.h>

    ;; Frequently used registers
.equ spl,  0x3d  ; Stack pointer low byte
.equ sph,  0x3e  ; Stack pointer high byte
.equ sreg, 0x3f  ; State register

    ;; Status register positions
.equ SREG_BIT_I, 7  ; Position of bit I (Global Interrupt) in status register
.equ SREG_BIT_T, 6  ; Position of bit T (Bit Copy Storage) in status register
.equ SREG_BIT_H, 5  ; Position of bit H (Half Carry Flag) in status register
.equ SREG_BIT_S, 4  ; Position of bit S (Sign Bit) in status register
.equ SREG_BIT_V, 3  ; Position of bit V (Overflow Flag) in status register
.equ SREG_BIT_N, 2  ; Position of bit N (Negative Flag) in status register
.equ SREG_BIT_Z, 1  ; Position of bit Z (Zero Flag) in status register
.equ SREG_BIT_C, 0  ; Position of bit C (Carry Flag) in status register

/**
 * Reset the kernel stack pointer to point to the end of RAM.
 *
 * @param register The register to use to store the address.
 */
.macro RESET_KERNEL_STACK_POINTER register
    ldi \register, hi8(_ramend)
    out sph, \register
    ldi \register, lo8(_ramend)
    out spl, \register
.endm

/**
 * Set the system status flag "in kernel" to 1, without preserving the
 * registers.
 *
 * @param register The register to use to perform the operation.
 */
.macro SET_SYSTEM_STATUS_IN_KERNEL register
    lds \register, systemStatus
    ori \register, POSITION(SYSTEM_STATUS_FLAG_IN_KERNEL_POSITION)
    sts systemStatus, \register
.endm

/**
 * Set the system status flag "in kernel" to 1, while preserving the registers.
 *
 * @param register The register to use to perform the operation.
 */
.macro SET_SYSTEM_STATUS_IN_KERNEL_PRESERVE_REGISTERS register
    push \register
    SET_SYSTEM_STATUS_IN_KERNEL \register
    pop \register
.endm

/**
 * Reset the system status flag "in kernel" to 0, without preserving the
 * registers.
 *
 * @param register The register to use to perform the operation.
 */
.macro UNSET_SYSTEM_STATUS_IN_KERNEL register
    lds \register, systemStatus
    andi \register, ~POSITION(SYSTEM_STATUS_FLAG_IN_KERNEL_POSITION)
    sts systemStatus, \register
.endm

/**
 * Reset the system status flag "in kernel" to 0, while preserving the
 * registers.
 *
 * @param register The register to use to perform the operation.
 */
.macro UNSET_SYSTEM_STATUS_IN_KERNEL_PRESERVE_REGISTERS register
    push \register
    UNSET_SYSTEM_STATUS_IN_KERNEL \register
    pop \register
.endm

/**
 * @file src/kern/arch/AVR/asm_common.S
 * @brief Common ASM macros and definitions.
 * @date Feb 2019
 * @author Remi Andruccioli
 *
 * This file defines commonly used AVR assembly macros and symbols in order to
 * be used in other ASM files with the .USING directive.
 */

    ;; Frequently used registers
.equ spl,  0x3d  ; Stack pointer low byte
.equ sph,  0x3e  ; Stack pointer high byte
.equ sreg, 0x3f  ; State register

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

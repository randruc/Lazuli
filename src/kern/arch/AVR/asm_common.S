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
.equ spl,  0x3d                ; Stack pointer low byte
.equ sph,  0x3e                ; Stack pointer high byte
.equ sreg, 0x3f                ; State register

/**
 * Reset the kernel stack pointer to point to the end of RAM.
 * Uses r16 and r17.
 */
.macro RESET_KERNEL_STACK_POINTER
    ldi r16, lo8(_ramend)
    ldi r17, hi8(_ramend)
    out spl, r16
    out sph, r17
.endm

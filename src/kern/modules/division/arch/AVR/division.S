/*
 * SPDX-License-Identifier: GPL-3.0-only
 * This file is part of Lazuli.
 */

/**
 * @file
 * @brief Unsigned integer division implementation on AVR.
 * @copyright 2020, Remi Andruccioli <remi.andruccioli@gmail.com>
 *
 * This file describes the implementation of unsigned division of 16-bit
 * integers onthe AVR architecture.
 *
 * @warning On the AVR platform, no hardware exception mechanism is provided.
 *          Hence division by zero will not cause any error! In that case, the
 *          returned struct will be filled with zeros.
 */

    ;; r24, r25 Numerator
    ;; r22, r23 Denominator
    .global Arch_Divide_U16
Arch_Divide_U16:
    push r19
    mov r19, r24
    or r19, r25
    brne test_denominator
    clr r22
    clr r23
    rjmp exit
test_denominator:
    mov r19, r22
    or r19, r23
    brne can_divide
    clr r24
    clr r25
    rjmp exit
can_divide:
    push r28
    push r29
    push r30
    push r31
divide:
    ldi r19, 16                 ; Initialize counter to sizeof(uint16)
    clr r28                     ; Initialize working area to zero
    clr r29                     ; (cont.)
    clc
shift_loop:
    rol r24
    rol r25
    rol r28
    rol r29
    movw r30, r28               ; Trial substraction:
    sec                         ;     - First, force the carry flag
    sub r30, r22                ;     - Then, perform the actual substraction
    sbc r31, r23                ;       (cont.)
    brcs ignore_result          ; Carry flag still set == Substraction failed
    movw r28, r30               ; Copy result of the division to working area
    dec r19                     ; If substraction succeed we need to push a 1
    sec                         ; to the quotient. So we do it here to avoid
    brne shift_loop             ; "dec r19" to mess everything.
    rjmp finish
ignore_result:
    dec r19
    clc
    brne shift_loop
finish:
    rol r24
    rol r25
    movw r22, r28
    pop r31
    pop r30
    pop r29
    pop r28
exit:
    pop r19
    ret


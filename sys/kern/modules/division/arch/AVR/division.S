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
 * integers on the AVR architecture.
 *
 * @warning On the AVR platform, no hardware exception mechanism is provided.
 *          Hence division by zero will not cause any error! In that case, the
 *          returned struct will be filled with zeros.
 */

/*
 * For more information on how it's done:
 * https://youtu.be/v3-a-zqKfgA
 * http://nparker.llx.com/a2/mult.html
 */

    ;; r24, r25: Numerator
    ;; r22, r23: Denominator
    ;; Working registers
    ;; r19: counter (sizeof uint16)
    ;; r28, r29: working area
    .global Arch_Divide_U16
Arch_Divide_U16:
    ;; If we attempt to divide by zero => Fail.
    tst r22
    brne can_divide
    tst r23
    brne can_divide
    call Kernel_ManageFailure   ; TODO: See if this 'call' should be replaced by
                                ;       a 'jmp'.
can_divide:
    push r19
    push r28
    push r29
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
    cp r28, r22
    cpc r29, r23
    brlo substraction_is_impossible
    sub r28, r22
    sbc r29, r23
    dec r19                     ; If substraction succeed we need to push a 1
    sec                         ; to the quotient. So we do it here to avoid
    brne shift_loop             ; "dec r19" to mess everything.
    rjmp finish
substraction_is_impossible:
    dec r19
    clc
    brne shift_loop
finish:
    rol r24
    rol r25
    movw r22, r28
    pop r29
    pop r28
    pop r19
    ret

/*
 * SPDX-License-Identifier: GPL-3.0-only
 * This file is part of Lazuli.
 */

/**
 * @file
 * @brief AVR routines of conversions from binary integer to ASCII decimal.
 * @copyright 2020, Remi Andruccioli <remi.andruccioli@gmail.com>
 *
 * This file describes the implementation of conversion from binary integer to
 * ASCII decimal in AVR ASM.
 */

/*
 * For more information on how it's done:
 * https://youtu.be/v3-a-zqKfgA
 * http://nparker.llx.com/a2/mult.html
 */
    ;; Input:
    ;;   r24, r25: input value
    ;;   r22, r23: pointer to the buffer
    ;;
    ;; Output:
    ;;   r24: Number of bytes written
    ;;
    ;; Working registers:
    ;;   r28, r29: working area
    ;;   r26, r27 (X): copy of input pointer
    ;;   r22 (reuse): counter (sizeof uint16)
    ;;   r23 (reuse): counter of final size of buffer (return value)
    .global Printf_ConvertU16ToDecimal
Printf_ConvertU16ToDecimal:
    push r26
    push r27
    push r28
    push r29
    movw r26, r22               ; Copy input pointer to register X
    clr r23                     ; Reuse r23 to initialize final size of buffer
divide:
    ldi r22, 16                 ; Initialize counter to sizeof(uint16)
    clr r28                     ; Initialize working area to zero
    clr r29                     ; (cont.)
    clc
shift_loop:
    rol r24
    rol r25
    rol r28
    rol r29
    cpi r28, 10                     ; We now check if we can substract 10 from
    brlo subtraction_is_impossible  ; the working area.
    sbiw r28, 10                ; If we are here, substraction is possible.
    dec r22                     ; We need to push a 1 to the quotient:
    sec                         ; we do it here (sec) to avoid "dec r22" to mess
    brne shift_loop             ; everything.
    rjmp convert_to_ascii
subtraction_is_impossible:
    dec r22
    clc
    brne shift_loop
convert_to_ascii:
    rol r24
    rol r25
    subi r28, -'0'              ; r28 += '0' (Convert remainder to ASCII
    st X+, r28                  ; and store it in the buffer)
    inc r23                     ; Increment final size of buffer
    sbiw r24, 0                 ; r24 == 0 ?
    brne divide                 ; We do another division while quotient != 0
    mov r24, r23                ; Set the return value (final size of buffer)
    pop r29
    pop r28
    pop r27
    pop r26
    ret

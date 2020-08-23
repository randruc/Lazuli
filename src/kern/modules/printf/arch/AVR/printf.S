/**
 * @file src/kern/modules/printf/arch/AVR/printf.S
 * @brief AVR routines of conversions from binary integer to ASCII decimal.
 * @date Aug 2020
 * @author Remi Andruccioli
 *
 * This file describes the implementation of conversion from binary integer to
 * ASCII decimal in AVR ASM.
 */

    /*
     * For more information on how it's done:
     * https://youtu.be/v3-a-zqKfgA
     * http://nparker.llx.com/a2/mult.html
     *
     * TODO: This code can be adapted to implement integer division, if needed.
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
    ;;   r30, r31: buffer for trial substraction
    ;;   r26, r27 (X): copy of input pointer
    ;;   r19: counter (sizeof uint16)
    ;;   r22 (reuse): constant '0'
    ;;   r23 (reuse): counter of final size of buffer (return value)
    .global Printf_ConvertU16ToDecimal
Printf_ConvertU16ToDecimal:
    push r19
    push r26
    push r27
    push r28
    push r29
    push r30
    push r31
    movw r26, r22               ; Copy input pointer to register X
    ldi r22, '0'                ; Reuse r22 to store the constant '0'
    clr r23                     ; Reuse r23 to initialize final size of buffer
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
    sbiw r30, 10                ;     - Then, perform the actual substraction
    brcs ignore_result          ; Carry flag still set == Substraction failed
    movw r28, r30               ; Copy result of the division to working area
    dec r19                     ; If substraction succeed we need to push a 1
    sec                         ; to the quotient. So we do it here to avoid
    brne shift_loop             ; "dec r19" to mess everything.
    rjmp convert_to_ascii
ignore_result:
    dec r19
    clc
    brne shift_loop
convert_to_ascii:
    rol r24
    rol r25
    add r28, r22                ; r28 += '0' (Convert remainder to ASCII
    st X+, r28                  ; and store it in the buffer)
    inc r23                     ; Increment final size of buffer
    sbiw r24, 0                 ; r24 == 0 ?
    brne divide                 ; We do another division while quotient != 0
    mov r24, r23                ; Set the return value (final size of buffer)
    pop r31
    pop r30
    pop r29
    pop r28
    pop r27
    pop r26
    pop r19
    ret

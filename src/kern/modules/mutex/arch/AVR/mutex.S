/*
 * SPDX-License-Identifier: GPL-3.0-only
 * This file is part of Lazuli.
 */

/**
 * @file
 * @brief AVR locks for mutexes implementation.
 * @copyright 2019-2020, Remi Andruccioli <remi.andruccioli@gmail.com>
 *
 * This file describes the implementation of locks for mutexes for AVR
 * architecture.
 *
 * This implementation is highly arch-specific because we must guarantee
 * atomicity of operations.
 * On AVR the only way to do that is by disabling/enabling interrupts.
 */

/*
 * TODO: Find a better way to perform that.
 * As it is a standard cpp #include, maybe this could be done by doing
 * basically the same a system headers (gcc -I).
 */
#include "../../../../arch/AVR/asm_common.S"

    .global Arch_TryAcquireLock
Arch_TryAcquireLock:
    push r30
    push r31
    movw r30, r24
    cli
    ld r24, Z
    tst r24
    breq lock_acquired
    sei
    ldi r24, 0
    rjmp exit
lock_acquired:
    ldi r24, 1
    st Z, r24
    sei
exit:
    pop r31
    pop r30
    ret

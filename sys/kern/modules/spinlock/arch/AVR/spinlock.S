/*
 * SPDX-License-Identifier: GPL-3.0-only
 * This file is part of Lazuli.
 */

/**
 * @file
 * @brief AVR spinlocks implementation.
 * @copyright 2019-2020, Remi Andruccioli <remi.andruccioli@gmail.com>
 *
 * This file describes the implementation of spinlocks for AVR architecture.
 *
 * This implementation is highly arch-specific because we must guarantee
 * atomicity of operations.
 * On AVR the only way to do that is by disabling/enabling interrupts.
 */

#include <config.h>

    .global Lz_Spinlock_Lock
Lz_Spinlock_Lock:
.IF LZ_CONFIG_CHECK_NULL_PARAMETERS_IN_SPINLOCKS
    sbiw r24, 0
    brne skip_abort
    call Task_Abort
skip_abort:
.ENDIF /* LZ_CONFIG_CHECK_NULL_PARAMETERS_IN_SPINLOCKS */
    push r30
    push r31
    movw r30, r24
try_acquire_lock:
    cli
    ld r24, Z
    tst r24
    breq lock_acquired
    sei ;; TODO: Change that to restore the previous state. Or think...
    rjmp try_acquire_lock
lock_acquired:
    ldi r24, 1
    st Z, r24
    sei ;; TODO: Change that to restore the previous state. Or think...
    pop r31
    pop r30
    ret

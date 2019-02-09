/**
 * @file src/kern/arch/AVR/spinlock.S
 * @brief Arch-specific spinlocks implementation.
 * @date Feb 2019
 * @author Remi Andruccioli
 *
 * Implementation of spinlocks for AVR architecture.
 */

#include <Lazuli/sys/config.h>

    .global Lz_Spinlock_Lock
Lz_Spinlock_Lock:
.IF CONFIG_CHECK_NULL_PARAMETERS_IN_SPINLOCKS
    sbiw r24, 0
    breq Arch_InfiniteLoop
.ENDIF /* CONFIG_CHECK_NULL_PARAMETERS_IN_SPINLOCKS */
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

    .global Lz_Spinlock_Unlock
Lz_Spinlock_Unlock:
.IF CONFIG_CHECK_NULL_PARAMETERS_IN_SPINLOCKS
    sbiw r24, 0
    breq Arch_InfiniteLoop
.ENDIF /* CONFIG_CHECK_NULL_PARAMETERS_IN_SPINLOCKS */
    push r30
    push r31
    movw r30, r24
    ldi r24, 0
    st Z, r24
    pop r31
    pop r30
    ret

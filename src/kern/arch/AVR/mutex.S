/**
 * @file src/kern/arch/AVR/mutex.S
 * @brief AVR locks for mutexes implementation.
 * @date Feb 2019
 * @author Remi Andruccioli
 *
 * This file describes the implementation of locks for mutexes for AVR
 * architecture.
 *
 * This implementation is highly arch-specific because we must guarantee
 * atomicity of operations.
 * On AVR the only way to do that is by disabling/enabling interrupts.
 */

#include <Lazuli/sys/config.h>

.INCLUDE "kern/arch/AVR/asm_common.S"

    .global Arch_TryAcquireLock
Arch_TryAcquireLock:
.IF LZ_CONFIG_CHECK_NULL_PARAMETERS_IN_MUTEXES
    sbiw r24, 0
infinite_loop:
    breq infinite_loop
.ENDIF /* LZ_CONFIG_CHECK_NULL_PARAMETERS_IN_MUTEXES */
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

    .global Arch_WaitMutex
Arch_WaitMutex:
    cli
    call save_context_on_stack
    movw r22, r24
    in r24, spl
    in r25, sph
    RESET_KERNEL_STACK_POINTER
    jmp BaseScheduler_WaitMutex

/**
 * @file startup.S
 *
 * Set up the runtime environment and run the kernel.
 * Defines AVR-specific routines implementation declared in Arch_XXX.
 */

#include <config.h>
    
#include <Lazuli/sys/arch/AVR/interrupts.h>

#include "asm_common.S"

/**
 * Declare the generic code for an interrupt handler.
 *
 * @param code The interrupt code defined in interrupts.h.
 */
.macro INTERRUPT_HANDLER code
    call save_context_on_stack
    ldi r22, \code
    jmp enter_kernel_handle_interrupt
.endm

    ;;
    ;; Interrupts handlers
    ;;
    .global int0_handler
int0_handler:
    INTERRUPT_HANDLER INT_INT0

    .global int1_handler
int1_handler:
    INTERRUPT_HANDLER INT_INT1

    .global pcint0_handler
pcint0_handler:
    INTERRUPT_HANDLER INT_PCINT0

    .global pcint1_handler
pcint1_handler:
    INTERRUPT_HANDLER INT_PCINT1

    .global pcint2_handler
pcint2_handler:
    INTERRUPT_HANDLER INT_PCINT2

    .global wdt_handler
wdt_handler:
    INTERRUPT_HANDLER INT_WDT

    .global timer2compA_handler
timer2compA_handler:
    INTERRUPT_HANDLER INT_TIMER2COMPA

    .global timer2compB_handler
timer2compB_handler:
    INTERRUPT_HANDLER INT_TIMER2COMPB

    .global timer2ovf_handler
timer2ovf_handler:
    INTERRUPT_HANDLER INT_TIMER2OVF

    .global timer1capt_handler
timer1capt_handler:
    INTERRUPT_HANDLER INT_TIMER1CAPT

    .global timer1compA_handler
timer1compA_handler:
    INTERRUPT_HANDLER INT_TIMER1COMPA

    .global timer1compB_handler
timer1compB_handler:
    INTERRUPT_HANDLER INT_TIMER1COMPB

    .global timer1ovf_handler
timer1ovf_handler:
    INTERRUPT_HANDLER INT_TIMER1OVF

    .global timer0compA_handler
timer0compA_handler:
    INTERRUPT_HANDLER INT_TIMER0COMPA

    .global timer0compB_handler
timer0compB_handler:
    INTERRUPT_HANDLER INT_TIMER0COMPB

    .global timer0ovf_handler
timer0ovf_handler:
    INTERRUPT_HANDLER INT_TIMER0OVF

    .global spistc_handler
spistc_handler:
    INTERRUPT_HANDLER INT_SPISTC

    .global usartrx_handler
usartrx_handler:
    INTERRUPT_HANDLER INT_USARTRX

    .global usartudre_handler
usartudre_handler:
    INTERRUPT_HANDLER INT_USARTUDRE

    .global usarttx_handler
usarttx_handler:
    INTERRUPT_HANDLER INT_USARTTX

    .global adc_handler
adc_handler:
    INTERRUPT_HANDLER INT_ADC

    .global eeready_handler
eeready_handler:
    INTERRUPT_HANDLER INT_EEREADY

    .global analogcomp_handler
analogcomp_handler:
    INTERRUPT_HANDLER INT_ANALOGCOMP

    .global twi_handler
twi_handler:
    INTERRUPT_HANDLER INT_TWI

    .global spmready_handler
spmready_handler:
    INTERRUPT_HANDLER INT_SPMREADY

/**
 * Switch to kernel stack and give hand to the scheduler to handle the
 * interrupt.
 *
 * Before calling this subroutine, you first need to fill r22 with the
 * interrupt code.
 */
enter_kernel_handle_interrupt:
    in r24, spl
    in r25, sph
    RESET_KERNEL_STACK_POINTER
    jmp Scheduler_HandleInterrupt

/**
 * Wait for a specific interrupt to occur.
 *
 * Technically:
 * - Disable interrupts
 * - Save the context of the current task
 * - Reset the stack to switch to kernel stack
 * - Give hand to the base scheduler by passing it the stack pointer of the
 *   current task and the interrupt code
 *
 * @param r24 A value of Lz_InterruptCode representing the interrupt
 *            to wait for.
 *
 * WARNING!!! This routine only works as long as r24 is left untouched until the
 *            line "mov r22, r24".
 */
    .global Lz_WaitInterrupt
Lz_WaitInterrupt:
    cli
    call save_context_on_stack
    mov r22, r24
    in r24, spl
    in r25, sph
    RESET_KERNEL_STACK_POINTER
    jmp Scheduler_WaitEvent

    .global Lz_Task_Terminate
Lz_Task_Terminate:
    cli
.IF LZ_CONFIG_SAVE_TASK_CONTEXT_ON_TERMINATION
    call save_context_on_stack
    in r24, spl
    in r25, sph
.ENDIF
    RESET_KERNEL_STACK_POINTER
    jmp Scheduler_ManageTaskTermination

    .global Task_Abort
Task_Abort:
    cli
    call save_context_on_stack
    in r24, spl
    in r25, sph
    RESET_KERNEL_STACK_POINTER
    jmp Scheduler_AbortTask

    ;; TODO: This label is global. See how we can rename it (Arch_XXX, Asm_XXX,
    ;; ...)
    /**
     * Save current running task execution context on task's stack.
     *
     * Consists in saving all registers (including the state register).
     * The program counter is automaticaly saved by hardware when vectoring
     * interrupt handler.
     */
    .global save_context_on_stack
save_context_on_stack:
    ;; Push Y, r27 and r26 registers
    push r29
    push r28
    push r27
    push r26
    ;; Store SP in Y register
    in r29, sph
    in r28, spl
    ;; Compute new SP to point to the memory location just before calling this
    ;; subroutine (4 previous pushes)
    adiw r28, 5
    ;; Save the return address of this subroutine in r26 and r27
    ld r26, Y
    ldd r27, Y+1

    adiw r28, 1

    ;; Set the new SP
    out sph, r29
    out spl, r28

    ;; We can now push r31 and r30 registers
    push r31
    push r30
    ;; Compute new SP to point to the memory location after pushing the 4
    ;; registers at the beginning
    sbiw r28, 6
    ;; Set the new SP
    out sph, r29
    out spl, r28
    ;; Push all other registers
    push r25
    push r24
    push r23
    push r22
    push r21
    push r20
    push r19
    push r18
    push r17
    push r16
    push r15
    push r14
    push r13
    push r12
    push r11
    push r10
    push r9
    push r8
    push r7
    push r6
    push r5
    push r4
    push r3
    push r2
    push r1
    push r0
    ;; Push state register
    in r16, sreg
    push r16
    ;; Push subroutine return address
    push r27
    push r26
    ret

    .global Arch_StartRunning
Arch_StartRunning:
    ;; Here we can skip POPing the 32 registers + the state register since they
    ;; have not been used yet. So we add the offset of the PC member of the
    ;; TaskContextLayout struct to the task stack pointer.
    add r24, r22
    adc r25, r23
    ;; We can now point to the saved return address (PC)
    out spl, r24
    out sph, r25
    reti

    .global Arch_RestoreContextAndReturnFromInterrupt
Arch_RestoreContextAndReturnFromInterrupt:
    ;; Switch to the task stack, passed by parameter
    out spl, r24
    out sph, r25
    ;; Restore state register
    pop r16
    out sreg, r16
    ;; Restore all registers
    pop r0
    pop r1
    pop r2
    pop r3
    pop r4
    pop r5
    pop r6
    pop r7
    pop r8
    pop r9
    pop r10
    pop r11
    pop r12
    pop r13
    pop r14
    pop r15
    pop r16
    pop r17
    pop r18
    pop r19
    pop r20
    pop r21
    pop r22
    pop r23
    pop r24
    pop r25
    pop r26
    pop r27
    pop r28
    pop r29
    pop r30
    pop r31
    ;;  Return from interrupt
    reti

    .global Arch_DisableInterrupts
Arch_DisableInterrupts:
    cli
    ret

    .global Arch_EnableInterrupts
Arch_EnableInterrupts:
    reti

    .global Arch_DisableInterruptsGetStatus
Arch_DisableInterruptsGetStatus:
    ;; The mask obtained by the interrupt bit in the status register.
    .equ IBitMask, 0x80
    in r24, sreg
    andi r24, IBitMask
    cli
    ret

    .global Arch_RestoreInterruptsStatus
Arch_RestoreInterruptsStatus:
    sbrc r24, SREG_BIT_I
    reti
    cli
    ret

    .global Arch_AreInterruptsEnabled
Arch_AreInterruptsEnabled:
    in r24, sreg
    andi r24, IBitMask
    ret

    /**
     * Copy bytes from program memory into RAM.
     *
     * @param r28, r29 (16 bits) The number of bytes to copy.
     * @param r30, r31 (16 bits) The source address in program memory of the
     *                           stored bytes.
     * @param r26, r27 (16 bits) The destination address in RAM.
     */
copy_from_program_memory:
    sbiw r28, 0
    breq copy_from_program_memory_exit
    push r17
copy_from_program_memory_loop:
    lpm r17, Z+
    st X+, r17
    sbiw r28, 1
    brne copy_from_program_memory_loop
    pop r17
copy_from_program_memory_exit:
    ret

    .global Arch_LoadFromProgmem
Arch_LoadFromProgmem:
    push r26
    push r27
    movw r26, r20
    sbiw r26, 0
    breq load_from_progmem_exit
    push r28
    push r29
    push r30
    push r31
    movw r30, r24
    movw r28, r22
load_from_progmem_loop:
    lpm r24, Z+
    st Y+, r24
    sbiw r26, 1
    brne load_from_progmem_loop
    pop r31
    pop r30
    pop r29
    pop r28
load_from_progmem_exit:
    pop r27
    pop r26
    ret

    .global Arch_LoadU8FromProgmem
Arch_LoadU8FromProgmem:
    push r30
    push r31
    movw r30, r24
    lpm r24, Z
    pop r31
    pop r30
    ret

    .global Arch_LoadPointerFromProgmem
Arch_LoadPointerFromProgmem:
    .global Arch_LoadFunctionPointerFromProgmem
Arch_LoadFunctionPointerFromProgmem:
    .global Arch_LoadU16FromProgmem
Arch_LoadU16FromProgmem:
    push r30
    push r31
    movw r30, r24
    lpm r24, Z+
    lpm r25, Z
    pop r31
    pop r30
    ret

    .global cpu_sleep
cpu_sleep:
    sleep
    ret

    /**
     * Main entry point of the whole system.
     */
    /* TODO: Implement Arch_ResetSystem with a watchdog reset */
    .global Arch_ResetSystem
Arch_ResetSystem:
    .global reset_system
reset_system:

    ;; TODO: Switch off the watchdog (in case of a software reset)

    ;; Disable interrupts
    cli

    RESET_KERNEL_STACK_POINTER

    ;; Clear bss section
    ;; TODO: Find a way to omit this code if no .bss
    ldi r28, lo8(_bss_size)
    ldi r29, hi8(_bss_size)
    sbiw r28, 0
    breq bss_loop_end
    ldi r26, lo8(_bss_start)
    ldi r27, hi8(_bss_start)
    ldi r17, 0
bss_loop_start:
    st X+, r17
    sbiw r28, 1
    brne bss_loop_start
bss_loop_end:

    ;; Load data section
    ;; TODO: Find a way to omit this code if no .data
    ldi r28, lo8(_data_size)
    ldi r29, hi8(_data_size)
    sbiw r28, 0
    breq load_data_end
    ldi r30, lo8(_data_load_start)
    ldi r31, hi8(_data_load_start)
    ldi r26, lo8(_data_start)
    ldi r27, hi8(_data_start)
    call copy_from_program_memory
load_data_end:

    ;; Load rodata section
    ;; TODO: Find a way to omit this code if no .rodata
    ldi r28, lo8(_rodata_size)
    ldi r29, hi8(_rodata_size)
    sbiw r28, 0
    breq load_rodata_end
    ldi r30, lo8(_rodata_load_start)
    ldi r31, hi8(_rodata_load_start)
    ldi r26, lo8(_rodata_start)
    ldi r27, hi8(_rodata_start)
    call copy_from_program_memory
load_rodata_end:

    ;; Start the kernel
    call Kernel_Main

    ;; Parachute trap
    ;; This should never be reached. It is only here is case of Kernel_Main
    ;; returns.
    call Kernel_Panic

    .global Arch_InfiniteLoop
Arch_InfiniteLoop:
    rjmp Arch_InfiniteLoop

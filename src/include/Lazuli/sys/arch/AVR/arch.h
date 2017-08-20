/**
 * @file src/include/Lazuli/sys/arch/AVR/arch.h
 *
 * Defines architecture specific functions for AVR.
 */

#ifndef LZ_ARCH_H
#define LZ_ARCH_H

#include <Lazuli/common.h>
#include <Lazuli/sys/compiler.h>

_EXTERN_C_DECL_BEGIN

/**
 * Enable global interrupts.
 */
extern void
global_interrupts_enable();

/**
 * Disable global interrupts.
 */
extern void
global_interrupts_disable();

/**
 * Function that loops forever, never returns.
 */
noreturn extern void
infinite_loop();

/**
 * Reset the whole system.
 */
extern void
reset_system();

/**
 * Restore the context of a previoulsy saved task, and run it by returning
 * from interrupt. The context of a task is saved on the task's stack.
 *
 * Consists in restoring all registers (including the state register) and
 * continue execution of the task by performing "reti", as the program
 * counter has previously been saved on the stack by hardware.
 *
 * @param stackPointer The stack pointer of the task to restore, containing its
 *                     saved context.
 */
extern void
restore_context_from_stack_and_reti(void *stackPointer);

/**
 * Start running the scheduler for the first time with the specified context.
 *
 * This function simply ignores all the registers in the context, and jumps
 * directly to the saved return address.
 *
 * @param stackPointer The stack pointer of the task to run, containing its
 *                     prepared context.
 * @param offsetOfPc   The offset of the PC member in the
 *                     TaskContextLayout structure (i.e. the saved context of
 *                     the task).
 */
extern void
start_running(void *stackPointer, size_t offsetOfPc);

/**
 * Swap bytes of a 16-bits value.
 *
 * @param value The value to swap (passed by copying its value).
 *
 * @return The swapped value.
 */
extern u16
swap16(u16 value);

/**
 * Return a byte stored in program memory.
 *
 * @param source A pointer to the address of the byte in program memory.
 *
 * @return The byte stored a the address contained in source parameter.
 */
extern u8
load_u8_from_progmem(char const * const source);

_EXTERN_C_DECL_END

#endif /* LZ_ARCH_H */

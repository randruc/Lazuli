/**
 * @file arch.h
 *
 * Defines architecture specific functions for AVR.
 */

#ifndef LZ_ARCH_H
#define LZ_ARCH_H

#include <Lazuli/common.h>

_EXTERN_C_DECL_BEGIN

/**
 * Enable global interrupts.
 */
extern void
GlobalInterruptsEnable();

/**
 * Disable global interrupts.
 */
extern void
GlobalInterruptsDisable();

/**
 * Function that loops forever, never returns.
 */
extern void
InfiniteLoop();

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
 */
extern void
start_running(void *stackPointer);

/**
 * Swap bytes of a 16-bits value.
 *
 * @param value The value to swap (passed by copying its value).
 *
 * @return The swapped value.
 */
extern u16
swap16(u16 value);


_EXTERN_C_DECL_END

#endif /* LZ_ARCH_H */

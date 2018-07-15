/**
 * @file src/include/Lazuli/sys/arch/arch.h
 * @brief Architecture specific functions.
 * @date Feb 2017
 * @author Remi Andruccioli
 *
 * Defines architecture specific functions.
 */

#ifndef LAZULI_SYS_ARCH_ARCH_H
#define LAZULI_SYS_ARCH_ARCH_H

#include <stdint.h>

#include <Lazuli/common.h>
#include <Lazuli/sys/compiler.h>

_EXTERN_C_DECL_BEGIN

/**
 * Function that loops forever, never returns.
 */
__noreturn extern void
infinite_loop();

/**
 * Reset the whole system.
 */
extern void
reset_system();

/**
 * @brief Restore the context of a previously saved task, and run it by
 * returning from interrupt.
 *
 * The context of a task is saved on its stack.
 * Consists in restoring all of the registers (including the state register) and
 * continue execution of the task by performing a `return from interrupt`, as
 * the program counter has previously been saved on the stack by hardware.
 *
 * @param stackPointer The stack pointer of the task to restore.
 */
extern void
restore_context_and_return_from_interrupt(void *stackPointer);

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
 * Return a byte stored in program memory.
 *
 * @param source A pointer to the address of the byte in program memory.
 *
 * @return The byte stored a the address contained in source parameter.
 */
extern uint8_t
load_u8_from_progmem(char const * const source);

_EXTERN_C_DECL_END

#endif /* LAZULI_SYS_ARCH_ARCH_H */

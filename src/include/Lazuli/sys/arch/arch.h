/**
 * @file src/include/Lazuli/sys/arch/arch.h
 * @brief Architecture Abstraction API.
 * @date Feb 2017
 * @author Remi Andruccioli
 *
 * Provides a simple abstraction API to architecture specific functions.
 * This is the API that must be re-implemented if porting to another platform.
 *
 * This one is taylored after the AVR platform and can be subject to change if
 * porting.
 * These functions are the only entry points to ASM code in the kernel.
 */

#ifndef LAZULI_SYS_ARCH_ARCH_H
#define LAZULI_SYS_ARCH_ARCH_H

#include <stdint.h>

#include <Lazuli/common.h>
#include <Lazuli/list.h>
#include <Lazuli/sys/compiler.h>

_EXTERN_C_DECL_BEGIN

/**
 * Function that loops forever, never returns.
 */
__noreturn extern void
Arch_InfiniteLoop();

/**
 * Reset the whole system.
 */
extern void
Arch_ResetSystem();

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
Arch_RestoreContextAndReturnFromInterrupt(void *stackPointer);

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
Arch_StartRunning(void *stackPointer, size_t offsetOfPc);

/**
 * Return a byte stored in program memory.
 *
 * @param source A pointer to the address of the byte in program memory.
 *
 * @return The byte stored a the address contained in source parameter.
 */
extern uint8_t
Arch_LoadU8FromProgmem(char const * const source);

/**
 * Disbale all interrupts.
 */
extern void
Arch_DisableInterrupts();

/**
 * Enable all interrupts.
 */
extern void
Arch_EnableInterrupts();

/**
 * Initialize all architecture-specific parameters.
 */
void
Arch_Init();

/**
 * Put the CPU to sleep according to the sleep settings.
 */
void
Arch_CpuSleep();

/**
 * Try to acquire a mutex by changing the value pointed by the lock parameter.
 *
 * @warning This function disables interrupts at the beginning of its execution.
 *          If the mutex could be acquired (i.e. returning false), then
 *          interrupts are enabled before returning to the caller.
 *          BUT if the mutex couldn't be acquired (i.e. returning true), then
 *          interrupts are left disabled before returning to the caller.
 *
 * @param lock A pointer to the lock.
 *
 * @return - false if the lock was free (then it could be obtained)
 *         - true if the lock wasn't free (then it couldn't be obtained)
 */
bool
Arch_TryAcquireMutex(volatile uint8_t * const lock);

/**
 * Stop the current running task and place it on the list of tasks waiting for
 * the mutex specified in parameter.
 *
 * @param waitingTasks A pointer to the list of tasks waiting for the mutex.
 */
void
Arch_WaitMutex(LinkedList * const waitingTasks);

_EXTERN_C_DECL_END

#endif /* LAZULI_SYS_ARCH_ARCH_H */

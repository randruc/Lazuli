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
 * porting to another platform.
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
Arch_InfiniteLoop(void);

/**
 * Reset the whole system.
 */
extern void
Arch_ResetSystem(void);

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
 * Disable all interrupts.
 */
extern void
Arch_DisableInterrupts(void);

/**
 * Enable all interrupts.
 */
extern void
Arch_EnableInterrupts(void);

/**
 * Initialize all architecture-specific parameters.
 */
void
Arch_Init(void);

/**
 * Put the CPU to sleep according to the sleep settings.
 */
void
Arch_CpuSleep(void);

/**
 * Try to acquire a lock by atomically changing the value pointed by the lock
 * parameter.
 *
 * @param lock A pointer to the lock.
 *
 * @return - true if the lock was free, hence it could be acquired
 *         - false if the lock wasn't free, hence it couldn't be acquired
 */
bool
Arch_TryAcquireLock(volatile uint8_t * const lock);

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

/*
 * SPDX-License-Identifier: GPL-3.0-only
 * This file is part of Lazuli.
 */

/**
 * @file
 * @brief Architecture Abstraction API.
 * @copyright 2017-2020, Remi Andruccioli <remi.andruccioli@gmail.com>
 *
 * Provides a simple abstraction API to architecture specific functions.
 * This is the API that must be re-implemented if porting to another platform.
 *
 * This one is tailored after the AVR platform and can be subject to change if
 * porting to another platform.
 */

#ifndef LAZULI_SYS_ARCH_ARCH_H
#define LAZULI_SYS_ARCH_ARCH_H

#include <stdint.h>

#include <Lazuli/common.h>
#include <Lazuli/list.h>
#include <Lazuli/serial.h>
#include <Lazuli/sys/compiler.h>

_EXTERN_C_DECL_BEGIN

/**
 * Function that loops forever, never returns.
 */
NORETURN void
Arch_InfiniteLoop(void);

/**
 * Reset the whole system.
 */
void
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
void
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
void
Arch_StartRunning(void *stackPointer, size_t offsetOfPc);

/**
 * Copy bytes from program memory to RAM.
 *
 * @param source A pointer to the source address in program memory.
 * @param destination A pointer to the destination address in RAM.
 * @param size The number of bytes to copy.
 */
void
Arch_LoadFromProgmem(const void * source,
                     void * destination,
                     const size_t size);

/**
 * Return a byte stored in program memory.
 *
 * @warning The return type is unsigned.
 *
 * @param source A pointer to the byte stored in program memory.
 *
 * @return The byte value stored at the address contained in source parameter.
 */
uint8_t
Arch_LoadU8FromProgmem(const void *source);

/**
 * Return a double-byte word stored in program memory.
 *
 * @warning The return type is unsigned.
 *
 * @param source A pointer to the word stored in program memory.
 *
 * @return The word value stored at the address contained in source parameter.
 */
uint16_t
Arch_LoadU16FromProgmem(const void *source);

/**
 * Return a pointer stored in program memory.
 *
 * @param source A pointer to the pointer stored in program memory.
 *
 * @return The pointer value stored at the address contained in source
 *         parameter.
 */
void *
Arch_LoadPointerFromProgmem(const void *source);

/**
 * Return a function pointer stored in program memory.
 *
 * @warning To use this function with a different prototype than
 *          void (*)(void), the return value must be cast to the appropriate
 *          function pointer type.
 *          We can't use Arch_LoadPointerFromProgmem() to do the same thing
 *          because ISO C forbids assignment between function pointer and
 *          'void *', and conversion of object pointer to function pointer type.
 *
 * @param source A pointer to the function pointer stored in program memory.
 *
 * @return The function pointer value stored at the address contained in source
 *         parameter.
 */
void (*Arch_LoadFunctionPointerFromProgmem(const void *source)) (void);

/**
 * Disable all interrupts.
 */
void
Arch_DisableInterrupts(void);

/**
 * Enable all interrupts.
 */
void
Arch_EnableInterrupts(void);

/* TODO: This is machine specific!!! */
/**
 * Define the type used to store interrupts status.
 *
 * You shall ALWAYS set a variable of this type by calling
 * Arch_DisableInterruptsGetStatus(), and NEVER modify its value manually.
 */
typedef uint8_t InterruptsStatus;

/**
 * Disable all interrupts and return the previous interrupts status.
 *
 * @return The previous global interrupts status.
 */
InterruptsStatus
Arch_DisableInterruptsGetStatus(void);

/**
 * Restore a previously saved interrupts status.
 *
 * @param interruptsStatus A previously saved InterruptsStatus.
 */
void
Arch_RestoreInterruptsStatus(const InterruptsStatus interruptsStatus);

/**
 * Obtain a value indicating if global interrupts are enabled.
 *
 * @return : - true if global interrupts are enabled
 *           - false if global interrupts are disabled
 */
bool
Arch_AreInterruptsEnabled(void);

/**
 * Initialize idle CPU modes.
 */
void
Arch_InitIdleCpuMode(void);

/**
 * Put the CPU to sleep according to the sleep settings.
 */
void
Arch_CpuSleep(void);

/**
 * Initialize the context switch instrumentation.
 */
void
Arch_InitInstrumentation(void);

/** @name System timer */
/** @{                 */

/**
 * Initialize the system timer.
 */
void
Arch_InitSystemTimer(void);

/**
 * Start the system timer.
 */
void
Arch_StartSystemTimer(void);

/** @}                 */

/** @name Mutex */
/** @{          */

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

/** @} */

/** @name Serial */
/** @{           */

/**
 * Retrieve the current configuration of the serial line.
 *
 * @param configuration A pointer to an allocated Lz_SerialConfiguration used to
 *                      store the configuration.
 */
void
Arch_GetSerialConfiguration(Lz_SerialConfiguration * const configuration);

/**
 * Configure the serial line according to the parameter.
 *
 * @param configuration A pointer to an existing Lz_SerialConfiguration
 *                      containing the full configuration to set up the serial
 *                      line.
 */
void
Arch_SetSerialConfiguration(const Lz_SerialConfiguration * const configuration);

/**
 * Initialize serial line with default configuration at system startup.
 */
void
Arch_InitSerial(void);

/**
 * Represents the result of a uin16_t division.
 */
typedef struct {
  uint16_t remainder; /**< The remainder of the division */
  uint16_t quotient;  /**< The quotient of the division  */
}U16DivisionResult;

/**
 * Perform the Euclidean division between two uint16_t operands.
 *
 * @param numerator The numerator of the division.
 * @param denominator The denominator of the division.
 *
 * @return A U16DivisionResult by value. The strange function prototype is
 *         inherited from libc's div().
 *
 * @warning As no hardware exception is provided on the AVR architecture. This
 *          function is written to never fail, even if you perform a division by
 *          zero. In that case the return value will be filled with zeros. So
 *          check the denominator before calling this function.
 */
U16DivisionResult
Arch_Divide_U16(uint16_t numerator, uint16_t denominator);

/** @} */

_EXTERN_C_DECL_END

#endif /* LAZULI_SYS_ARCH_ARCH_H */

/**
 * @file src/include/Lazuli/sys/scheduler_base.h
 * @brief Base scheduler interface. The kernel API of the base scheduler.
 * @date Feb 2017
 * @author Remi Andruccioli
 *
 * This file describes the common interface of every scheduler classes in
 * Lazuli.
 * It defines the interface to which each scheduler class must conform.
 * It also defines symbols that are accessible to any scheduler class.
 */

#ifndef LAZULI_SYS_SCHEDULER_BASE_H
#define LAZULI_SYS_SCHEDULER_BASE_H

#include <stdint.h>

#include <Lazuli/common.h>
#include <Lazuli/lazuli.h>

#include <Lazuli/sys/task.h>

_EXTERN_C_DECL_BEGIN

/**
 * A pointer to the current running task.
 */
extern Task *currentTask;

/**
 * Reverse the bytes of a function pointer.
 *
 * @param pointer A function pointer.
 *
 * @return A new function pointer from reversed bytes of parameter "pointer".
 */
void
(*ReverseBytesOfFunctionPointer(void (* const pointer)(void)))(void);

/**
 * Initialize the scheduler prior to running it.
 * This function is called by kernel initialization.
 */
void
Scheduler_Init(void);

/**
 * Manage the termination of a task.
 *
 * This function is called when a task terminates its execution.
 * i.e. when the task returns from its main function of when it calls
 * Lz_Task_Terminate().
 *
 * @param sp The stack pointer of the terminating task.
 *           This parameter will be used only if configuration option
 *           LZ_CONFIG_SAVE_TASK_CONTEXT_ON_TERMINATION is set to 1.
 */
void
Scheduler_ManageTaskTermination(void * const sp);

/**
 * Call the appropriate scheduler to abort the curent running task.
 *
 * @param sp The stack pointer of the running task after saving its context.
 */
void
Scheduler_AbortTask(void * const sp);

/*
 * TODO: Maybe rename interruptCode and eventCode to xxxxxId or something like
 * that...
 */
/**
 * This function is called by arch-specific interrupt handling routine, after
 * saving the context of the current running task.
 *
 * @param sp The stack pointer of the current running task after saving its
 *           context.
 * @param interruptCode The code of the interrupt being handled.
 */
void
Scheduler_HandleInterrupt(void * const sp, const uint8_t interruptCode);

/**
 * This function is called from arch-specific Wait routine in order to get the
 * current running task wait for a particular event, after saving its context.
 *
 * @param sp The stack pointer of the current running task after saving its
 *           context.
 * @param eventCode The code of the event the task is waiting for.
 */
void
Scheduler_WaitEvent(void * const sp, const uint8_t eventCode);

/**
 * Wake up all tasks waiting for a mutex.
 *
 * @param waitingTasks A pointer to the Lz_LinkedList containing all the tasks
 * waiting for the mutex.
 */
void
Scheduler_WakeupTasksWaitingMutex(Lz_LinkedList * const waitingTasks);

/**
 * Place the current running task in the queue of tasks waiting for a mutex.
 * This function is called from arch-specific WaitMutex routine in order to get
 * the current running task wait for a mutex, after saving its context.
 *
 * @param sp The stack pointer of the current running task after saving its
 *           context.
 * @param waitingTasks A pointer to the Lz_LinkedList containing all the tasks
 * waiting for the mutex.
 */
void
Scheduler_WaitMutex(void * const sp, Lz_LinkedList * const waitingTasks);

_EXTERN_C_DECL_END

#endif /* LAZULI_SYS_SCHEDULER_BASE_H */

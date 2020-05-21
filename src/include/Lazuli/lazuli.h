/**
 * @file src/include/Lazuli/lazuli.h
 * @brief The public API of the Lazuli kernel.
 * @date Feb 2017
 * @author Remi Andruccioli
 *
 * This file describes the public API of the Lazuli kernel.
 * It defines public types and functions that can by used by user main code
 * and tasks.
 */

#ifndef LAZULI_LAZULI_H
#define LAZULI_LAZULI_H

#include <stdint.h>

#include <Lazuli/common.h>
#include <Lazuli/config.h>

_EXTERN_C_DECL_BEGIN

/**
 * Represents the priority of a task, as a signed integer.
 * The higher the value, the higher the priority.
 */
typedef int8_t lz_task_priority_t;

/**
 * Represents the type used for the system clock resolution unit, as an unsigned
 * integer.
 */
typedef uint16_t lz_u_resolution_unit_t;

/**
 * Defines the possible scheduling policies for a Lazuli user task.
 *
 * @attention The constants are defined from the highest priority policy to the
 *            lowest. i.e. A lower value means a higher priority.
 */
enum Lz_SchedulingPolicy {
  /**
   * @cond false
   *
   * Undocumented to user: only here for static verification.
   * This entry MUST be the first one.
   */
   __LZ_SCHEDULING_POLICY_ENUM_BEGIN = -1,

   /**
    * Cyclic real-time scheduling.
    */
   CYCLIC_RT = 0,

   /**
    * Priority time sliced real-time scheduling.
    *
    * Equivalent to POSIX SCHED_RR.
    */
   PRIORITY_RT,

   /**
    * Priority round-robin scheduling (non real-time).
    */
   /*
    * PRIORITY_RR,
    */
   
  /**
   * @cond false
   *
   * Undocumented to user: only here for static verification.
   * This entry MUST be the last one.
   */
   __LZ_SCHEDULING_POLICY_ENUM_END
};

/**
 * Represents the configuration of a task.
 */
typedef struct {
  /**
   * A pointer to an allocated const string containing the name to give to the
   * task.
   * That string must NOT be deallocated after registering the task.
   */
  char const *name;

  /**
   * The size of the stack needed by the task
   */
  size_t stackSize;

  /**
   * The scheduling policy of the task.
   */
  enum Lz_SchedulingPolicy schedulingPolicy;

  /**
   * The priority of task. Only used for non-cyclic tasks.
   * The lower this number is, the higher the priority will be.
   */
  lz_task_priority_t priority;
  
  /**
   * The period (T) of the task. Used only for cyclic tasks.
   *
   * The period is expressed as an integer number of time units.
   */
  lz_u_resolution_unit_t period;

  /**
   * The completion time (C) of the task (worst case execution time). Used only
   * for cyclic tasks.
   *
   * The completion time is expressed as an integer number of time units.
   */
  lz_u_resolution_unit_t completion;
}Lz_TaskConfiguration;

/* TODO: Maybe move this one somewhere else. */
/**
 * Represents a message that a Task can pass to the scheduler after its time
 * slice has expired.
 */
enum TaskToSchedulerMessage {
  /**
   * @cond false
   *
   * Undocumented to user: only here for static verification.
   * This entry MUST be the first one.
   */
  __TASK_TO_SCHEDULER_MESSAGE_ENUM_BEGIN = -1,

  /**
   * No message has to be passed to the scheduler.
   */
  NO_MESSAGE = 0,

  /**
   * Set the task to wait for its next activation.
   * i.e. It finnished its work without consuming all of its completion time.
   */
  WAIT_ACTIVATION,

  /**
   * Set the task to wait for an interrupt.
   * A parameter representing the interrupt number must accompany this message.
   */
  WAIT_INTERRUPT,

  /**
   * Terminate the task.
   */
  TERMINATE_TASK,

  /**
   * @cond false
   *
   * Undocumented to user: only here for static verification.
   * This entry MUST be the last one.
   */
  __TASK_TO_SCHEDULER_MESSAGE_ENUM_END
};

/**
 * Register a new task.
 *
 * If an error occured during registration of the task _false_ is returned and
 * the task is not included in the set of tasks that will be run.
 *
 * @param taskEntryPoint The entry point of the task to register.
 *                       i.e. A pointer to the function representing the task.
 * @param taskConfiguration A pointer to an Lz_TaskConfiguration containing the
 *                          configuration of the task being registered.
 *                          If NULL is passed, then default values are applied
 *                          for all parameters.
 *
 * @return
 *         - _true_ if the task has been registered without error.
 *         - _false_ if an error occured during registration.
 */
bool
Lz_RegisterTask(void (* const taskEntryPoint)(void),
                Lz_TaskConfiguration * taskConfiguration);

/**
 * @brief Initialize an Lz_TaskConfiguration with default values for all
 *        parameters.
 *
 * No function is provided for allocating a new Lz_TaskConfiguration.
 * So it is strongly advised to allocate the Lz_TaskConfiguration parameter
 * on the stack before calling this function.
 *
 * @param taskConfiguration A pointer to the Lz_TaskConfiguration to initialize.
 */
void
Lz_TaskConfiguration_Init(Lz_TaskConfiguration * const taskConfiguration);

/**
 * Run the scheduler.
 *
 * Start scheduling tasks.
 */
void
Lz_Run(void);

/**
 * Wait for a specific interrupt to occur.
 * Puts the calling task to sleep until the specified interrupt occurs.
 *
 * @param interruptCode The code of the interrupt to wait for.
 *
 * @attention Only tasks with scheduling policy PRIORITY_RT can wait for
 *            interrupts.
 */
void
Lz_Task_WaitInterrupt(uint8_t interruptCode);

/**
 * Get the name of the calling task.
 *
 * @return A pointer to a string containing the name of the current running
 *         task, or NULL if the task has no name.
 */
char const *
Lz_Task_GetName(void);

/**
 * Terminate the calling task. The context of the task will be saved on its
 * stack.
 *
 * Calling this function has the same effect than returning from the task's main
 * function.
 *
 * The terminated task will never be scheduled again.
 */
void
Lz_Task_Terminate(void);

/**
 * Set the calling task to wait for its next activation.
 * May be used if the task finnished its work without consuming all of its
 * completion time.
 *
 * @attention Only tasks with scheduling policy CYCLIC_RT can wait for next
 *            activation.
 */
void
Lz_Task_WaitActivation(void);

_EXTERN_C_DECL_END

#endif /* LAZULI_LAZULI_H */

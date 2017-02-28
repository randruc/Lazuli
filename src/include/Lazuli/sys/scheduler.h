/**
 * @file scheduler.h
 *
 * This file describes the kernel interface for the scheduler.
 */

#ifndef LZ_SYS_SCHEDULER_H
#define LZ_SYS_SCHEDULER_H


extern void
restore_context_from_stack_and_reti();

/**
 * Initialize the scheduler.
 */
void
Scheduler_Init();

#endif /* LZ_SYS_SCHEDULER_H */

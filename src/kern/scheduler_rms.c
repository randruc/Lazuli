/**
 * @file src/kern/scheduler_rms.c
 * @brief Rate Monotonic Scheduler implementation.
 * @date May 2019
 * @author Remi Andruccioli
 *
 * This file describes the implementation of the Rate Monotonic tasks scheduler.
 */

#include <Lazuli/common.h>
#include <Lazuli/lazuli.h>
#include <Lazuli/list.h>

#include <Lazuli/sys/arch/arch.h>
#include <Lazuli/sys/arch/AVR/interrupts.h>
#include <Lazuli/sys/clock_24.h>
#include <Lazuli/sys/compiler.h>
#include <Lazuli/sys/kernel.h>
#include <Lazuli/sys/memory.h>
#include <Lazuli/sys/scheduler.h>
#include <Lazuli/sys/scheduler_rms.h>
#include <Lazuli/sys/task.h>

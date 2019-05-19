/**
 * @file src/kern/sizeof_types.c
 * @brief Sizeof types.
 * @date Mar 2018
 * @author Remi Andruccioli
 *
 * This file is used to retrieve the size of various types in the kernel after
 * compilation.
 * The resulting object file (.o) of this C file is excluded from linking into
 * the final binary image.
 */

#include <Lazuli/common.h>
#include <Lazuli/mutex.h>
#include <Lazuli/spinlock.h>
#include <Lazuli/sys/scheduler_base.h>
#include <Lazuli/sys/scheduler_hpf.h>
#include <Lazuli/sys/scheduler_rr.h>
#include <Lazuli/sys/task.h>

/**
 * Declare a constant containing the size of a given type.
 *
 * @param T The type of which to get the size.
 * @param M An short informative message to display what this type size means.
 */
#define SIZEOF_TYPE(T, M)                        \
  const volatile size_t                          \
  __sizeof_##T = sizeof(T);                      \
  const volatile char *                          \
  __message_##T = M

/** @cond false */

SIZEOF_TYPE(TaskContextLayout,
            "RAM needed to save the context of a task.");

SIZEOF_TYPE(SchedulerOperations,
            "RAM needed to hold operations for a single scheduler.");

SIZEOF_TYPE(Task,
            "RAM needed for a Task (base).");

SIZEOF_TYPE(RrTask,
            "RAM needed for an RR task.");

SIZEOF_TYPE(HpfTask,
            "RAM needed for an HPF task.");

SIZEOF_TYPE(Lz_Spinlock,
            "RAM needed for an Lz_Spinlock.");

SIZEOF_TYPE(Lz_Mutex,
            "RAM needed for an Lz_Mutex.");

/** @endcond */

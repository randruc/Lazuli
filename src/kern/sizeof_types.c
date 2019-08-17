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

#include <Lazuli/clock_24.h>
#include <Lazuli/common.h>
#include <Lazuli/lazuli.h>
#include <Lazuli/mutex.h>
#include <Lazuli/spinlock.h>
#include <Lazuli/sys/scheduler.h>
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

SIZEOF_TYPE(Clock24,
            "RAM needed for a Clock24.");

SIZEOF_TYPE(Lz_Mutex,
            "RAM needed for an Lz_Mutex.");

SIZEOF_TYPE(Lz_Spinlock,
            "RAM needed for an Lz_Spinlock.");

SIZEOF_TYPE(Lz_TaskConfiguration,
            "RAM needed for Lz_TaskConfiguration.");

SIZEOF_TYPE(Task,
            "RAM needed for a Task (base).");

SIZEOF_TYPE(TaskContextLayout,
            "RAM needed to save the context of a task.");

/** @endcond */

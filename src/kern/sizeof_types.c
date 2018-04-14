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
#include <Lazuli/sys/task.h>
#include <Lazuli/sys/scheduler_rr.h>
#include <Lazuli/sys/scheduler_hpf.h>

/**
 * Declare a constant containing the size of a given type.
 *
 * @param X The type of which to get the size.
 * @param M An short informative message to display what this type size means.
 */
#define SIZEOF_TYPE(T, M)                        \
  const volatile size_t                          \
  __sizeof_##T = sizeof(T);                      \
  const volatile char *                          \
  __message_##T = M

SIZEOF_TYPE(TaskContextLayout,
            "RAM needed to save the context of a task.");

SIZEOF_TYPE(Task,
            "Basic size of a task.");

SIZEOF_TYPE(RrTask,
            "RAM needed for an RR task.");

SIZEOF_TYPE(HpfTask,
            "RAM needed for an HPF task.");

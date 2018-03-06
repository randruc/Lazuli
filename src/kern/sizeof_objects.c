/**
 * @file src/kern/sizeof_objects.c
 * @brief Sizeof objects.
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
#include <Lazuli/lazuli.h>

static const volatile size_t
__sizeof_Task = sizeof(Task);

static const volatile size_t
__sizeof_Lz_TaskConfiguration = sizeof(Lz_TaskConfiguration);

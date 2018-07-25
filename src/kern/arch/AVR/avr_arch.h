/**
 * @file src/kern/arch/AVR/avr_arh.h
 * @brief Header file specific to the AVR arch module.
 * @date Jul 2018
 * @author Remi Andruccioli
 *
 * This header file declares all symbol neeed specifically in the AVR arch
 * module, and not declared in <Lazuli/sys/arch/arch.h>.
 */

#ifndef LAZULI_SRC_KERN_ARCH_AVR_ARCH_H
#define LAZULI_SRC_KERN_ARCH_AVR_ARCH_H

#include <Lazuli/common.h>

_EXTERN_C_DECL_BEGIN

/**
 * Put the AVR CPU to sleep according to the sleep settings.
 */
extern void
cpu_sleep();

_EXTERN_C_DECL_END

#endif /* LAZULI_SRC_KERN_ARCH_AVR_ARCH_H */

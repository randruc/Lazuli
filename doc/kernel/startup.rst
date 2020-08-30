..
   SPDX-License-Identifier: GPL-3.0-only
   This file is part of Lazuli.
   Copyright (c) 2020, Remi Andruccioli <remi.andruccioli@gmail.com>

.. sectionauthor:: Remi Andruccioli <remi.andruccioli@gmail.com>

System startup
==============

System startup is written is assembly language, in the file
``src/kern/arch/AVR/startup.S``. It contains the necessary code to launch the
kernel. The main goal of system startup routines is to set up a working C
runtime environment.

When powered on the AVR MCU starts executing code at address 0x00 in ROM.
This entry point is defined as the reset interrupt handler (written in
``src/kern/arch/AVR/interrupt_vectors_table.S``). This interrupt handler
performs a jump to the ASM routine ``reset_system``.

The routine ``reset_system`` will first set the stack pointer to the end of RAM.
This is the stack that will be used during all the initialization process.
The ``reset_system`` routine will then set up the appropriate sections in RAM:

* bss
* data
* rodata

After these operations, we now have a working C runtime.
The ``reset_system`` routine will finally give hand to C code, by calling the
C function ``Kernel_Main`` defined in ``src/kern/kernel.c``.

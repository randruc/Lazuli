..
   SPDX-License-Identifier: GPL-3.0-only
   This file is part of Lazuli.

.. sectionauthor::
   Copyright (c) 2020, Remi Andruccioli <remi.andruccioli@gmail.com>

System startup
==============

Assembly - Setting up a C runtime
---------------------------------

System startup is divided in 2 parts:

* The first part, written in assembly language, executes right after powering
  on the system, and has the responsibility to set up an operational C runtime.
* The second part, mainly written in C, performs various operations that are
  necessary to initialize the data structures of the kernel, before giving hand
  to user code.

Initial system startup is written is assembly language, in the file
``sys/kern/arch/AVR/startup.S``. It contains the necessary code to launch the
kernel. The main goal of system startup routines is to set up a working C
runtime environment.

When powered on the AVR MCU starts executing code at address 0x0000 in ROM.
This entry point is defined as the reset interrupt handler (written in
``sys/kern/arch/AVR/interrupt_vectors_table.S``). This interrupt handler
performs a jump to the ASM routine ``reset_system``. This routine makes heavy
use of symbols defines by the linker script (in the file
``sys/kern/linker.ld``).

First of all, the routine ``reset_system`` will make sure that global interrupts
are disabled, then set the stack pointer to the end of RAM.
This stack that will be used during all the initialization process.
We will then set up the appropriate sections in RAM, in the following order:

* ``bss``: Set all the section to zero.
* ``data``: Copy data from ROM.
* ``rodata``: Copy data from ROM.

After these operations, we now have an operational C runtime. That's all for the
assembly part.
The ``reset_system`` routine will finally give hand to C code, by calling the
C function ``Kernel_Main`` defined in ``sys/kern/kernel.c``.

C - Initializing kernel structures
----------------------------------

We are now in ``Kernel_Main``.


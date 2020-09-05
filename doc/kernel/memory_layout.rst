..
   SPDX-License-Identifier: GPL-3.0-only
   This file is part of Lazuli.

.. sectionauthor::
   Copyright (c) 2020, Remi Andruccioli <remi.andruccioli@gmail.com>

Memory Layout
=============

Here is documented the memory layout for the kernel and user tasks.

In the context of AVR MCUs the term RAM used in this document refers to the MCU
SRAM, and the term ROM refers to the MCU flash memory.

Memory layout is implemented in the file ``src/kern/linker.ld``.

Harvard architecture and static linking
---------------------------------------

AVR MCUs use a Harvard architecture. Machine instructions and program data are
stored in separate memories.

Due to the particular architecture of AVR MCUs (especially the absence of an
MMU) the kernel and user tasks are all linked in a single executable file.
What we call "user" tasks are pieces of code written independently of the
kernel, and that are not distributed with it.
Therefore all the object files (kernel and user tasks) are statically linked
and share the same memory sections.

We explain here how these sections are loaded into ROM and how the RAM is
subdivided according to the sections needed at runtime.

ROM
***

The following sections are loaded in ROM:

* **.text**: contains all the executable code, i.e. the interrupt vectors table
  and the machine instructions of the user tasks and the kernel.
  On the AVR architecture the interrupt vectors table must be located at address
  0x0000.

* **.progmem**: contains all constants that are stored in ROM. These
  constants can be any type of data (strings, structs, ...). In C code, a
  constant is marked to be stored in ROM using the ``PROGMEM`` macro. When a
  constant is needed during execution, it is loaded in RAM at runtime using
  special AVR ``lpm`` instruction.

* **.data**: contains all global variables that are initialized in C.
  The .data section doesn't contains machine code but program data so it needs
  to be loaded in RAM at startup (see Startup.md).

* **.rodata**: contains all global variables that are initialized in C and
  marked as readonly using the keyword ``const``.
  The .rodata section doesn't contain machine code but program data so it needs
  to be loaded in RAM at startup (see Startup.md).

This graph shows how these sections are loaded into ROM.
The arrows on the right specify addresses.

.. code-block:: none

    +----------------+
    |. . . . . . . . |<- SIZEOF(ROM) - 1
    | . . . . . . . .|
    |. (unused). . . |
    | . . . . . . . .|
    |. . . . . . . . |
    | . . . . . . . .|<- _rodata_load_start + SIZEOF(.rodata)
    +----------------+
    |                |<- _rodata_load_start + SIZEOF(.rodata) - 1
    |  .rodata       |
    |  section       |   _data_load_start + SIZEOF(.data)
    |                |<- (_rodata_load_start in the linker)
    +----------------+
    |                |<- _data_load_start + SIZEOF(.data) - 1
    |  .data         |
    |  section       |   _progmem_load_start + SIZEOF(.progmem)
    |                |<- (_data_load_start) in the linker)
    +----------------+
    |                |<- _progmem_load_start + SIZEOF(.progmem) - 1
    |  .progmem      |
    |  section       |
    |                |<- SIZEOF(.text) (_progmem_load_start in the linker)
    +----------------+
    |                |<- SIZEOF(.text) - 1
    |                |
    |  .text         |
    |  section       |
    |                |
    |                |
    +-  -  -  -  -  -+
    | interrupt      |
    | vectors        |
    | table          |<- 0x0 
    +----------------+ 

RAM
***

A small part of the RAM is mapped to machine registers and I/O registers.
The usable memory part starts at address 0x100 and contains the following
regions:

* **.data**: This section is fixed in size and is copied from ROM during
  startup. It contains all initialized global variables.
* **.rodata**: This section is fixed in size and is copied from ROM during
  startup. It contains all global constants.
* **.bss**: This section is fixed in size and contains all uninitialized global
  variables in C. It is set to zero during startup.
* **.noinit**: This section contains all variables marked with the macro
  ``NOINIT``. This macro is used to indicated that an uninitialized global
  variable must not be initialized to zero at startup. 
* The **heap**: This is the kernel heap. After kernel startup the heap size is
  0.
* The **kernel stack**: During system startup only the kernel stack exists.
  On the AVR architecture the stack grows downward.
  The stack pointer points to the next free memory location that will be used
  when performing a ``push``.
* The space between the heap and the stack is left unused. This space isn't
  fixed in size because both the heap and the stack can grow downward or
  upward at runtime.

This graph shows how these sections are loaded into RAM:  
The arrows on the right specify addresses.

.. code-block:: none

    +----------------+
    |                |<- SIZEOF(RAM) - 1 (_ramend in the linker)
    | stack          |
    |                |
    +----------------+
    |. . . . . . . . |<- stack pointer (known at runtime)
    | (unused). . . .|
    |. . . . . . . . |
    | . . . . . . . .|<- break (known at runtime)
    +----------------+
    |                |
    | heap           |
    |                |<- heap_start (_brk in the linker)
    +----------------+ 
    | .noinit section|
    | (non zero'd at |
    | startup)       |   
    |                |<- 0x100 + SIZEOF(.data) + SIZEOF(.rodata) + SIZEOF(.bss)
    +----------------+ 
    | .bss section   |<- 0x100 + SIZEOF(.data) + SIZEOF(.bss) - 1
    | (zero'd at     |
    | startup)       |   0x100 + SIZEOF(.data) + SIZEOF(.rodata)
    |                |<- (_bss_start in the linker)
    +----------------+
    | .rodata section|<- 0x100 + SIZEOF(.data) + SIZEOF(.rodata) - 1
    | (loaded from   |
    | ROM)           |<- 0x100 + SIZEOF(.data) (_rodata_start in the linker)
    +----------------+
    | .data section  |<- 0x100 + SIZEOF(.data) - 1
    | (loaded from   |
    | ROM)           |<- 0x100 (_data_start in the linker)
    +----------------+
    | machine and    |<- 0xff
    | I/O registers  |
    |                |<- 0x0
    +----------------+

Allocating user tasks
---------------------

The allocation of new user tasks is done in the function
Lz_Scheduler_RegisterTask().
When registering a new user task the kernel must allocate enough space to
contain the `Task` object that represents a task. The kernel then allocates
enough space for the task's stack.
The reserved space for a task's stack is fixed in size, so a user task cannot
grow its stack bigger than the size asked when registering.

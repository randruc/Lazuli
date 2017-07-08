# Memory Layout

Here is documented the memory layout of the kernel and user tasks.


## Harvard architecture and static linking

AVR MCUs use a Harvard architecure. Machine instructions and program data are
stored in separate memories.

Due to the particular architecture of AVR MCUs the kernel and user tasks are
all linked in a single executable file. What we call "user" tasks are pieces of
code written independently of the kernel, and that are not distributed with it.
Therefore all the object files (kernel and user tasks) are statically linked
and share the same .text, .data, and .bss sections.

We explain here how these sections are loaded into ROM and how the RAM is
subdivised according to the sections needed at runtime.


### ROM:

The ROM contains 2 sections: .text and .data.  
The .text section contains the interrupt vectors table and the machine
instructions of the user tasks and the kernel.
On the AVR architecture the interrupt vectors table must be located at address
0x0.  
The .data section contains all global variables that are initialized in C.
This .data section doesn't contains machine code but program data so it needs
to be loaded in RAM at startup (see Startup.md).

This graph shows how these sections are loaded into ROM:  
The arrows on the right specify addresses.

    +----------------+
    |. . . . . . . . |<- SIZEOF(ROM) - 1
    | . . . . . . . .|
    |. . . . . . . . |
    | . . . . . . . .|
    |. (unused). . . |
    | . . . . . . . .|<- SIZEOF(.text) + SIZEOF(.data)
    +----------------+
    |                |<- SIZEOF(.text) + SIZEOF(.data) - 1
    |  .data         |
    |  section       |
    |                |<- SIZEOF(.text) (_data_load_start in the linker)
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


### RAM:

The RAM sections are mainly contitionned by the AVR architecture and the
runtime of the C language.

A small part of the RAM is mapped to machine registers and I/O registers.
The usable memory part starts at address 0x100 and contains the following
regions:
* .data section: This section is fixed in size and is copied from ROM during
  startup.
* .bss section: This section is fixed in size and contains all uninitialized
  global variables in C. It is set to zero during startup.
* The heap: This is the kernel heap. On startup the heap size is 0.
* The stack: This is the kernel stack. On the AVR architecture the stack grows
  downward.
* The space between the heap and the stack is left unused. This space isn't
  fixed in size because both the heap and the stack can grow downward or
  upward at runtime.

This graph shows how these sections are loaded into RAM:  
The arrows on the right specify addresses.

    +----------------+
    |                |<- SIZEOF(RAM) - 1 (_ramend in the linker)
    | stack          |
    |                |
    |                |<- stack pointer (known at runtime)
    +----------------+
    |. . . . . . . . |
    | (unused). . . .|
    |. . . . . . . . |
    | . . . . . . . .|<- break (known at runtime)
    +----------------+
    |                |
    | heap           |
    |                |
    |                |<- heap_start (_brk in the linker)
    +----------------+ 
    | .bss section   |<- 0x100 + SIZEOF(.data) + SIZEOF(.bss) - 1
    | (zero'd at     |
    | startup)       |<- 0x100 + SIZEOF(.data) (_bss_start in the linker)
    +----------------+
    | .data section  |<- 0x100 + SIZEOF(.data) - 1
    | (loaded from   |
    | ROM)           |<- 0x100 (_data_start in the linker)
    +----------------+
    | machine and    |<- 0xff
    | I/O registers  |
    |                |<- 0x0
    +----------------+


## User tasks memory

User tasks share their .text, .bss and .data section with other tasks and the
kernel as they are all fixed in size and can be determined at compile time.
Dynamic regions of user tasks include heap and stack.
When launching a task the kernel must therefore allocate into its own heap a
memory region that will contain both the heap and the stack for the user task.
The kernel needs to allocate sufficient memory in prevision to the usage of
dynamic memory allocation and stack by the user task.

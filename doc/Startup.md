# Startup

Here is documented the startup process.

This document explains more about the file `startup.S` which contains all
necessary code to launch the kernel.
The file `startup.S` is written in assembly language. Its goal is mainly to set
up the runtime environement needed to run a C program.

When powered on the AVR MCU starts executing code at address 0x0 in ROM.
The entry point is defined as the reset interrupt handler. This interrupt
handler performs a jump to the ASM routine `reset_system` (as written in
`interrupt_vectors_table.S`).

## Steps

Here are the steps performed by `reset_system` to initialize the system.

1. Disable interrupts. First of all, we ensure that global interrupts are
   disabled.
   
2. Set kernel stack pointer. The kernel stack pointer is set using the _ramend
   symbol defined by the linker. After this step, the kernel stack pointer
   points to the last address in RAM.

3. Clear the .bss section. The .bss section is cleared using the _bss_size and
   _bss_start symbols defined by the linker. All bytes within this section are
   set to zero.

4. Load the .data section. The .data section is copied to RAM from ROM using the
   _data_load_start (address in ROM), _data_start (address in RAM) and
   _data_size symbols defined by the linker.

5. Load the .rodata section. The .rodata section is copied to RAM from ROM using
   the _rodata_load_start (address in ROM), _rodata_start (address in RAM) and
   _rodata_size symbols defined by the linker.

6. Start the kernel. The kernel main entry point (KMain) is then called.

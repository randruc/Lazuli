Startup
=======

Here is documented the startup process.

This document explains more about the file startup.S which contains all
necessary code to launch the kernel.
The file startup.S is written is assembly. Its goal is mainly to set up the
runtime environement needed to run a C program.

The entry point is defined of the reset interrupt handler.

Steps
-----

1. Clear the .bss section. The .bss section is cleared using the _bss_size and
   _bss_start symbols defined by the linker.

2. Load the .data section. The .data section is copied to RAM from ROM using the
   _data_load_start (address in ROM), _data_start (address in RAM) and _data_size
   symbols defined by the linker.

3. Set kernel stack pointer. The kernel stack pointer is set using the _ramend
   symbols defined by the linker. After this step, the kernel stack pointer
   points to the last address in RAM.

4. Start the kernel. The kernel main entry point (KMain) is then called.

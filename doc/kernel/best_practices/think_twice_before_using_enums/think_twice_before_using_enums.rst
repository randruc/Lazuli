..
   SPDX-License-Identifier: GPL-3.0-only
   This file is part of Lazuli.

.. sectionauthor::
   Copyright (c) 2020, Remi Andruccioli <remi.andruccioli@gmail.com>

Think twice before using enumerations in C
==========================================

It is generally considered a good practice to define named integer constants
as enumerations (``enum``) if they are related to each other. i.e.: they can be
grouped under a common name, bringing more meaning, more "semantic intention":

.. literalinclude:: enum_declaration.c
    :language: c
    :lines: 10-

as opposed to preprocessor's ``#define`` that cannot group a set of macro
constants under a common name:

.. literalinclude:: define_declaration.c
    :language: c
    :lines: 10-

Most of the time and for the vast majority of usual C code, this practice should
be applied.
Enumerations also have the advantage of being true C symbols from the compiler
and debugger point of view.

However, there are a few situations where manipulating integer constants as
enumerations should not be considered, or at least should be taken really
carefully.
These situations happen when enumerations are manipulated as l-values on
specific architectures.

The C specification
-------------------

This is an excerpt from the C specification about enumerations (in
`<http://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf>`_ - 6.7.2.2
Enumeration specifiers):

    *"The expression that defines the value of an enumeration constant shall be
    an integer constant expression that has a value representable as an int."*

As every C programmer knows, the size of an ``int`` in C is left unspecified by
the standard.
This means that the size of a enumeration constant value is
platform/architecture/compiler dependant.
Enumerations are not of a guaranteed size by the C specification, and there is
no standard way to do so.

A consequence on binary interfacing
-----------------------------------

The first consequence of the C specification is that ``enum`` constants don't
have a fixed size across different architectures, as the size of an ``int``
varies between architectures/compilers.
This is quite a well-know fact amongst C programmers.

Defining constants as ``enum`` should be avoided when binary
interaction/interfacing is needed accross different architectures
(e.g. over networks).

An argument against *specific* uses of enumerations
---------------------------------------------------

Let's now introduce a sneaky pitfall, that happens *under specific conditions*.
Lazuli RTOS is targeting embedded systems. It is written in ANSI C and aims to
be easily portable across different architectures.

We take here the example of the AVR architecture, which is a target for the
Lazuli RTOS.

AVR is an 8-bit CPU, but on which many compilers (such as AVR-GCC) define the
type ``int`` to be 16-bit long.
This means that enumerations will be 16-bit long *as well*.

Here is our general rule:

    **Great care must be taken when using enumerations on architectures where
    the size of the machine word is narrower than the size of an int.**

Although this situation is usually not a problem (we've used 64-bit variables on
32-bit machines for years), it can become one in certain contexts, and can even
lead to synchronization problems as we will see.
And it's a situation that happens quite often in embedded C development, as we
are often dealing with tiny machines.
It is much more difficult to see these problems and their consequences when
using enumerations rather than integers, because we often rely on standard
header ``<stdint.h>`` when declaring integer variables. We then have a
convenient way to master the size of integer variables right from their
declaration.
Unfortunately no equivalent exists for enumerations, and its not easy to spot
their size at a glance when reviewing the source code.

Using enumeration variables on a platform where the size of the machine word is
narrower than the size of an int has 3 main consequences, that are in fact
linked together:

* **An impact on performance**
* **An impact on memory usage and the total size of the resulting binary**
* **Non atomicity of memory accesses**

Now let's see why.

We consider the following piece of C code (the constant values have been chosen
so we can find them easily within the disassembly):

.. literalinclude:: enum_example.c
    :language: c
    :lines: 10-

Notice that all these constants are only 8-bit long.

Let's compile this short example for the AVR architecture and observe the
assembly generated by the compiler.

.. code-block:: console
   :emphasize-lines: 12,13

    [root@0ca45bbdd5b1 best_practices]# avr-gcc -c -O3 enum_example.c \
    > && avr-objdump -d enum_example.o

    enum_example.o:     file format elf32-avr


    Disassembly of section .text:

    00000000 <example>:
       0:   83 ec           ldi r24, 0xC3   ; 195
       2:   90 e0           ldi r25, 0x00   ; 0
       4:   90 93 00 00     sts 0x0000, r25 ; 0x800000 <__SREG__+0x7fffc1>
       8:   80 93 00 00     sts 0x0000, r24 ; 0x800000 <__SREG__+0x7fffc1>
       c:   08 95           ret

First, the value of the constant ``EARTH`` (``0xc3``) is loaded in 2 registers
``r24`` and ``r25`` (``ldi``, LoaD-Immediate).
Notice here that one register (``r25``) is loaded with zero, as it is the high
byte of ``0xc3`` encoded as a 16-bit word.
Then this value is stored in memory, the corresponding lines are highlighted.
You can also notice that, even with compiler optimizations switched on
(``-O3``), the actual writing in memory is done in 2 operations (``sts``,
Store-To-Sram), one byte at a time.
This is coherent, as we are storing a 16-bit value in memory using an 8-bit CPU.

Now let's do the same thing, but by replacing the ``enum`` for a smaller integer
type that fits the architecture word size, and using the C preprocessor to
define our named constants:

.. literalinclude:: enum_example_with_cpp.c
    :language: c
    :lines: 10-

Let's examine the disassembly:

.. code-block:: console
   :emphasize-lines: 11

    [root@0ca45bbdd5b1 best_practices]# avr-gcc -c -O3 enum_example_with_cpp.c \
    > && avr-objdump -d enum_example_with_cpp.o

    enum_example_with_cpp.o:     file format elf32-avr


    Disassembly of section .text:

    00000000 <example>:
       0:   83 ec           ldi r24, 0xC3   ; 195
       2:   80 93 00 00     sts 0x0000, r24 ; 0x800000 <__SREG__+0x7fffc1>
       6:   08 95           ret

The actual writing in memory is now performed in one operation.
Needless to precise that this writing operation is atomic on the AVR.

We can now address our three concerns from before:

First, about **performance**.
This is a strong concern if you are writing code that must be *fast* in those
very architecture-specific cases (which is the case of many embedded projects,
such as Lazuli RTOS, or cross-architecture projects). If the values of your
``enum`` can all fit within a machine word, then you should consider using
``#define`` with a machine word size instead of ``enum``.
In the Lazuli kernel, all enumerations definitions used in the scheduler have
been replaced by ``typedef`` and ``#define`` to a smaller type.

Then, about the **memory usage and total size of binary**.
It is obvious when comparing the two disassemblies above that using enumerations
where a smaller type can be used leads to a bigger memory usage.
This is true for ROM (program memory), as well as RAM usage (global variables,
stack variables, etc.).
The difference may only be one byte, but the AVR ATmega328P only has 2048 bytes
of RAM, so one byte *is* important.
Obviously, this can be applied only if the values of your ``enum`` can all fit
within a machine word.

And last but not least, about the **atomicity of memory accesses**.
Looking at the first disassembly above, a context switch or an interrupt handler
can occur during writing in memory, between the two ``sts``.
If another thread of execution tries to access the value while the writing
thread is suspended between the two ``sts`` of its writing operation, then the
reader thread  will possibly read a corrupted value.
This can have catastrophic consequences.
This means that *in cases where the size of the machine word is narrower than
the size of an int*, enumerations *must not* be used to pass signals or
messages between different threads of execution.
Otherwise synchronization issues can occur.

Replacing enumerations by ``typedef`` and ``#define`` to a type whose size fits
the machine word solves this problem.

In those cases, you can use the type ``u_read_write_atomic_t`` that is defined
in ``<Lazuli/common.h>``.
The type ``u_read_write_atomic_t`` is an unsigned integer having a size that
guarantees that reading/writing from/to memory is atomic accross all
architectures.
This is the equivalent of the C standard ``sig_atomic_t`` defined in
``<signal.h>``.
In some situations, using this type can also avoid using an explicit lock.

This type is used in the Lazuli kernel any time a user task needs to pass
a message to the scheduler/kernel.

A note on ``-fshort-enums``
---------------------------

Some compilers support an option to produce "short enumerations", such as
GCC's ``-fshort-enums``.
This option is not used in Lazuli, as it compiler-specific.
Lazuli is written in standard C89, so it must compile and run correctly with any
compiler. *No assumptions about the compiler, no surprises.*
The code **is not** supposed to work correctly only under the condition of being
compiled with a specific compiler (especially for atomic reads and writes).

Some other problems can occur when using ``-fshort-enum``, but it is not worth
speaking about them here, as we care about not being compiler-specific.
You can read more about the pitfalls of using ``-fshort-enums`` here:
`<https://interrupt.memfault.com/blog/best-and-worst-gcc-clang-compiler-flags#-fshort-enum>`_

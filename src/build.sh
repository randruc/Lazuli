#!/bin/bash

##
# @brief Main build file.
#
# The role of this file is to:
# Build the executable and copy it as intel hex format binary image.
# * Compile kernel code to object files
# * Generate the Lazuli kernel from object files as a static library
# * Compile user code and statically link it with Lazuli kernel to create an
#   executable ELF file
# * Extract HEX image from ELF file to upload to target
#
# No Makefile for the moment.
#

project_name=Lazuli
debug=true

if [ $debug = true ]
then
    cflags='-g -DLZ_DEBUG=1'
else
    cflags='-DLZ_DEBUG=0'
fi

rm -f sourcelist
rm -f $project_name.elf
rm -f $project_name.hex
rm -f *.o
rm -f *.a

../checklines.sh

avr-gcc \
    -c \
    $cflags \
    -ansi \
    -std=c89 \
    -pedantic \
    -Wall \
    -Wextra \
    -Werror \
    -Iinclude \
    -mmcu=atmega328p \
    -O2 \
    -nostartfiles \
    -nostdlib \
    -nodefaultlibs \
    -fshort-enums \
    kern/arch/AVR/interrupt_vectors_table.S \
    kern/arch/AVR/startup.S \
    kern/arch/AVR/timer_counter_0.c \
    kern/arch/AVR/usart.c \
    kern/kernel.c \
    kern/memory.c \
    kern/scheduler_base.c \
    kern/scheduler_rr.c \
    kern/scheduler_hpf.c \
    kern/list.c \
    kern/sizeof_types.c

ar rcs lib$project_name.a \
    interrupt_vectors_table.o \
    startup.o \
    timer_counter_0.o \
    usart.o \
    kernel.o \
    memory.o \
    scheduler_base.o \
    scheduler_rr.o \
    scheduler_hpf.o \
    list.o \

avr-gcc \
    $cflags \
    -ansi \
    -std=c89 \
    -pedantic \
    -Wall \
    -Wextra \
    -Werror \
    -Iinclude \
    -mmcu=atmega328p \
    -O2 \
    -nostartfiles \
    -nostdlib \
    -nodefaultlibs \
    -fshort-enums \
    -T kern/linker.x \
    -o $project_name.elf \
    kern/main.c \
    lib$project_name.a

if [ -e $project_name.elf ]
then
    avr-objcopy \
        -j .text \
        -j .data \
        -j .rodata \
        -j .progmem \
        -O ihex \
        $project_name.elf \
        $project_name.hex

    # TODO: This doesn't seem to display correct sizes (rodata...)
    size -Adt Lazuli.elf | ./sizeof_sections.awk
    echo
    avr-objdump -j .data -D sizeof_types.o | ./sizeof_types.awk

    if [ $debug = true ]
    then
        avr-objdump -hS $project_name.elf > sourcelist
    fi
fi

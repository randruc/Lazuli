#!/bin/bash

# Main build file.
# Build the executable and copy it as intel hex format binary image.
# No Makefile for the moment.

project_name=Lazuli
debug=true

if [ $debug = true ]
then
    cflags='-g -DLZ_DEBUG=1'
else
    cflags=''
fi

rm -f sourcelist
rm -f $project_name.elf
rm -f $project_name.hex

../checklines.sh

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
    kern/arch/AVR/interrupt_vectors_table.S \
    kern/arch/AVR/startup.S \
    kern/arch/AVR/timer_counter_0.c \
    kern/arch/AVR/usart.c \
    kern/kernel.c \
    kern/memory.c \
    kern/scheduler.c \
    kern/list.c \
    kern/main.c

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

    if [ $debug = true ]
    then
        avr-objdump -hS $project_name.elf > sourcelist
    fi
fi

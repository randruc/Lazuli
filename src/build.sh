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

user_file=kern/unit-tests/unit_tests.c
#user_file=../example-programs/highest_priority_first.c
#user_file=../example-programs/mutex_2.c
#user_file=../example-programs/mutex.c
#user_file=../example-programs/round_robin.c
#user_file=../example-programs/spinlocks.c

debug=true

config_use_mutex=1
config_use_spinlock=1
config_use_serial=1

CC=avr-gcc

if [ $debug = true ]
then
    cflags='-g -DLZ_DEBUG=1'
else
    cflags='-DLZ_DEBUG=0'
fi

cflags=$cflags' -ansi'
cflags=$cflags' -std=c89'
cflags=$cflags' -pedantic'
cflags=$cflags' -Wall'
cflags=$cflags' -Wextra'
cflags=$cflags' -Werror'
cflags=$cflags' -Iinclude'
cflags=$cflags' -Ilibc-headers/arch-dependent/AVR'
cflags=$cflags' -mmcu=atmega328p'
cflags=$cflags' -O2'
cflags=$cflags' -nostartfiles'
cflags=$cflags' -nostdlib'
cflags=$cflags' -nodefaultlibs'
cflags=$cflags' -ffreestanding'
cflags=$cflags' -fshort-enums'
cflags=$cflags' -DLZ_CONFIG_USE_MUTEX='$config_use_mutex
cflags=$cflags' -DLZ_CONFIG_USE_SPINLOCK='$config_use_spinlock
cflags=$cflags' -DLZ_CONFIG_USE_SERIAL='$config_use_serial

echo $cflags | sed 's/ /\n/g'

./clean.sh > /dev/null

../checklines.sh

$CC $cflags -c kern/arch/AVR/arch.c \
    -o arch.o
$CC $cflags -c kern/arch/AVR/interrupt_vectors_table.S\
    -o interrupt_vectors_table.o
$CC $cflags -c kern/arch/AVR/startup.S \
    -o startup.o
$CC $cflags -c kern/arch/AVR/spinlock.S \
    -o arch_spinlock.o
$CC $cflags -c kern/spinlock.c \
    -o spinlock.o
$CC $cflags -c kern/arch/AVR/timer_counter_0.c \
    -o timer_counter_0.o
$CC $cflags -c kern/arch/AVR/usart.c \
    -o usart.o
$CC $cflags -c kern/kernel.c \
    -o kernel.o
$CC $cflags -c kern/memory.c \
    -o memory.o
$CC $cflags -c kern/scheduler_base.c \
    -o scheduler_base.o
$CC $cflags -c kern/scheduler_rr.c \
    -o scheduler_rr.o
$CC $cflags -c kern/scheduler_hpf.c \
    -o scheduler_hpf.o
$CC $cflags -c kern/list.c \
    -o list.o
$CC $cflags -c kern/mutex.c \
    -o mutex.o
$CC $cflags -c kern/arch/AVR/mutex.S \
    -o arch_mutex.o
$CC $cflags -c kern/serial.c \
    -o serial.o
$CC $cflags -c kern/sizeof_types.c \
    -o sizeof_types.o
$CC $cflags -c libc/stdint_assertions.c \
    -o stdint_assertions.o

object_files=''
object_files=$object_files' arch.o'
object_files=$object_files' interrupt_vectors_table.o'
object_files=$object_files' startup.o'
object_files=$object_files' timer_counter_0.o'
object_files=$object_files' kernel.o'
object_files=$object_files' memory.o'
object_files=$object_files' scheduler_base.o'
object_files=$object_files' scheduler_rr.o'
object_files=$object_files' scheduler_hpf.o'
object_files=$object_files' list.o'

if [ $config_use_spinlock -eq 1 ]
then
    object_files=$object_files' spinlock.o'
    object_files=$object_files' arch_spinlock.o'
fi

if [ $config_use_mutex -eq 1 ]
then
   object_files=$object_files' mutex.o'
   object_files=$object_files' arch_mutex.o'
fi

if [ $config_use_serial -eq 1 ]
then
    object_files=$object_files' usart.o'
    object_files=$object_files' serial.o'
fi

echo
echo $object_files | sed 's/ /\n/g' | sort
echo

ar rcs lib$project_name.a $object_files

$CC \
    $cflags \
    -T kern/linker.x \
    -o $project_name.elf \
    $user_file \
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
    size -Adt Lazuli.elf | ./sizeof_sections.awk > /dev/tty
    echo
    avr-objdump -j .data -D sizeof_types.o | ./sizeof_types.awk > /dev/tty

    if [ $debug = true ]
    then
        avr-objdump -hS $project_name.elf > sourcelist
    fi
fi

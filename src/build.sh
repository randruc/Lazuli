#!/bin/bash

# Main build file.
# Build the executable and copy it as intel hex format binary image.
# No Makefile for the moment.

project_name=Lazuli

avr-gcc \
	-ansi \
	-std=c89 \
	-pedantic \
	-Wall \
	-Wextra \
	-Werror \
	-Iinclude \
	-mmcu=atmega328p \
	-nostartfiles \
	-nostdlib \
	-nodefaultlibs \
	-T kern/linker.x \
	-o $project_name \
	kern/interrupt_vectors_table.S \
	kern/startup.S \
	kern/kernel.c \
	kern/memory.c \
	kern/arch.c \
	kern/main.c

avr-objcopy \
	-j .text \
	-j .data \
	-O ihex \
	$project_name \
	$project_name.hex

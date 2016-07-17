#!/bin/bash
# Main build file
# No Makefile for the moment

avr-gcc \
	-Iinclude \
	-mmcu=atmega328p \
	-nostartfiles \
	-nostdlib \
	-nodefaultlibs \
	-T kern/linker.x \
	-o u \
	kern/interrupt_vectors_table.S \
	kern/startup.S \
	kern/kmain.c \
	kern/malloc.c \
	kern/arch.c

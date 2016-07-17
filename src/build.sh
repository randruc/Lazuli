#!/bin/bash
# Main build file
# No Makefile for the moment

project_name=Peacock

avr-gcc \
	-Iinclude \
	-mmcu=atmega328p \
	-nostartfiles \
	-nostdlib \
	-nodefaultlibs \
	-T kern/linker.x \
	-o $project_name \
	kern/interrupt_vectors_table.S \
	kern/startup.S \
	kern/kmain.c \
	kern/malloc.c \
	kern/arch.c

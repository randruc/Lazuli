#!/bin/bash
# Main build file
# No Makefile for the moment

project_name=Lorenzo

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

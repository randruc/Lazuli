..
   SPDX-License-Identifier: GPL-3.0-only
   This file is part of Lazuli.

.. sectionauthor::
   Copyright (c) 2020, Remi Andruccioli <remi.andruccioli@gmail.com>

Introduction
============

*What is Lazuli, and what it is not.*

Lazuli is a preemptive real-time multitasking kernel targeting microcontrollers,
or machines with constrained resources. It allow the microcontroller to run
multiple independent tasks simultaneously, with some of them having hard
real-time constraints.
Lazuli provides a "time slice" scheduler in order to respect deadlines
constraints of the tasks running in the system.

For now only the ATmega328p is supported.

Lazuli is self-contained: it does not rely on an existing library.

Lazuli currently provides the following functionalities :

* **"ROMable"**: i.e. All the system can fit in ROM.
  Lazuli does not rely on the presence of a disk or storage device.
* **Real-time scheduling**: Tasks can be scheduled in a cyclic real-time Rate
  Monotonic Scheduling (RMS) fashion, or in a real-time priority round robin
  fashion (equivalent of POSIX SCHED_RR).
* **No MMU**: Lazuli does not relies on MMU or virtual memory.
  It runs on a unique flat address space, traditionally found in
  microcontrollers.
* **Modular**: Lazuli build system lets you choose which parts of the system you
  need, including only those necessary parts in the final binary, thus saving
  storage.
* **Containerized development environment**: An official container image is
  provided, that includes all the necessary tools to build the system and your
  own application.

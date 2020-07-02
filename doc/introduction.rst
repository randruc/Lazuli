.. sectionauthor:: Remi Andruccioli <remi.andruccioli@gmail.com>
.. date: May 2020


Introduction
============

What is Lazuli, and what it is not.

Lazuli is a multitasking kernel targeting microcontrollers or machines with
constrained resources.

For now only the ATMega 328p is supported.

Lazuli currently provides the following functionalities :

* **"ROMable"**: i.e. All the system can fit in ROM.
  Lazuli does not relies on the presence of a disk.
* **Realtime scheduling**: Tasks can be scheduled in a cyclic Rate Monotonic
  Scheduling (RMS) fashion, or in a realtime priority round robin fashion
  (equivalent of POSIX SCHED_RR).
* **No MMU**: Lazuli does not relies on MMU or virtual memory.
  It runs on a unique flat address space, traditionally found in
  microcontrollers.

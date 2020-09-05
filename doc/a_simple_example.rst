..
   SPDX-License-Identifier: GPL-3.0-only
   This file is part of Lazuli.

.. sectionauthor::
   Copyright (c) 2020, Remi Andruccioli <remi.andruccioli@gmail.com>

A simple example
================

Instead of the traditionnal "Hello world", we show here a simple clock
application.

This application demonstrates a simple clock that prints the time every second
on the serial port.

.. literalinclude:: ../example-programs/clock24.c
    :language: c

In this code sample, the ``main`` function is executed at system startup. In
this function we configure the serial port to enable serial transmission and
configure the speed of the transmission. Then we configure a single task to run.
The entry point for this task is the function ``ClockTask``. We configure this
task to be real-time cyclic, with a period of 50 time units (time slices), and a
completion time of 25 time units (time slices).

..
   SPDX-License-Identifier: GPL-3.0-only
   This file is part of Lazuli.

.. sectionauthor::
   Copyright (c) 2020, Remi Andruccioli <remi.andruccioli@gmail.com>

Set up the development environment
==================================

In order to develop with Lazuli you need to set up a development environment.
For that it is strongly recommended to use the official Lazuli Docker image.

You can choose to install your own tools on your machine, however it is not
guaranteed that the code will compile correctly.
The Docker image integrates the exact versions needed for Lazuli to compile
correctly.
There are many advantages of using the official Lazuli Docker image:

#. The development of the Lazuli kernel itself is made using the official
   Lazuli Docker image.
   So no guarantee is given for the code to work as expected when compiled with
   versions of the tools that are different than the versions provided in the
   Lazuli Docker image.
#. As the Docker image is updated with the rest of the source code in the
   repository, you have the guarantee that the development environment always
   uses the right versions of the tools. Your development environment will keep
   updated with the successive versions of the repository.
#. You do not have anything to install on your development machine.
#. Using the Docker image allows you to develop with Lazuli whether your
   machine runs Linux, Windows or macOS.

In the rest of this documentation, we consider that you are using the official
Lazuli Docker image.


About the Lazuli Docker image
-----------------------------

You need to keep in mind a few things about the Lazuli Docker image.

The Docker image is used *interactively*.
When the container is run, it starts a bash session that keeps alive for your
whole development session.
The user is root inside the container.

The Docker image uses a Fedora base image. This is why the Lazuli Docker image
can be quite big.
The image *does* integrate all the tools needed to configure build settings,
to compile the kernel and your application, and to analyze the produced
binaries.
The Docker image *does not* integrate any text editor and no tool to upload
the produced binary to the target machine.
It does not integrate Git neither.
Code editing and Git operations are done with your own tools, installed on your
own host machine.

Finally, the Docker image does not integrate any copy of the repository or your
own source code.
Instead, your own copy of the repository will be mapped inside the running
container.
The file system of the Lazuli container is not persistent.



Getting the Lazuli Docker image
-------------------------------

Pulling the official image
^^^^^^^^^^^^^^^^^^^^^^^^^^

You can use any tool that is compatible with OCI containers,
like Docker or podman:

.. code-block:: console

    docker pull randruc/lazuli:latest

Building your own image
^^^^^^^^^^^^^^^^^^^^^^^

Run the container
-----------------

Run the container with the following options.
Remember to replace ``/path/to/your/Lazuli/directory`` with the actual path to
your copy of the repository:

.. code-block:: console

    docker run --name lazuli -ti --rm -v/path/to/your/Lazuli/directory:/~/workspace:z randruc/lazuli

You should see a message, followed by a bash prompt:

.. code-block:: console

    $ docker run --name lazuli -ti --rm -v$(pwd):/~/workspace:z randruc/lazuli
    Welcome in the Lazuli development environment container.
    For Lazuli version 0.1.0.
    This image was generated on Mon May 11 20:09:37 UTC 2020.
    Fedora release 31 (Thirty One)
    Linux e654b59f6c18 5.6.13-100.fc30.x86_64 #1 SMP Fri May 15 00:36:06 UTC 2020 x86_64 x86_64 x86_64 GNU/Linux

    [root@e654b59f6c18 workspace]# 

The second line of the message indicates the corresponding version of Lazuli
the image is built for. It should match the version number in the file named
VERSION at the root of your copy of the Lazuli repository. If it doesn't,
you are not using the appropriate Docker image, and you should pull the
appropriate tag.

The fourth line indicates the Fedora version of the image.

The fifth line indicates the host system you are actually using.

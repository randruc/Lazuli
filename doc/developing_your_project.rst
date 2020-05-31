.. sectionauthor:: Remi Andruccioli <remi.andruccioli@gmail.com>
.. date: May 2020


Developing your project
=======================

Your application code
---------------------

At the root of the repository is a directory named ``user``.
This is where your own source code will go.
A default file named ``main.c`` is provided.
If you need to add more source files to your project, you must declare them
in the file ``CMakeLists.txt`` at the root of the repository: you will add your
files to the list named ``LAZULI_USER_SOURCE_FILES``.

Building
---------

The next step is to build your project.

The Lazuli kernel itself is not executable.
It is developed as a static library that your own program will link against at
compile time. This is why the ``main`` function that you can see in the file
``main.c`` is the *real* main function of the final program.



The whole Lazuli project relies on CMake for configuration and compilation.
As Lazuli is designed to be cross platform and configurable, we will explain
here how to configure CMake to your specific project before building it.

Configuration
^^^^^^^^^^^^^

The first step is to make CMake generate the appropriate cross compilation
toolchain for your project. This toolchain depends on the target platform.

We will do that in an interactive fashion using the tool ``ccmake``.
It is recommended that you perform these operations in the Lazuli container.

We first create and browse the directory ``build`` which is the destination
directory of build artefacts, and we invoke ``cmake`` by pointing to the root
``CMakeFile.txt``

.. code-block:: console

    [root@6bf01305461f workspace]# mkdir build && cd build
    [root@6bf01305461f build]# cmake ..
    CMake Error at src/cmake/machine_choice.cmake:23 (message):
    Configuration error: The target machine must be defined with cache variable
    'LZ_TARGET_MACHINE_CHOICE'.
    Call Stack (most recent call first):
    src/CMakeLists.txt:23 (include)


    -- Configuring incomplete, errors occurred!
    [root@6bf01305461f build]# 

This results in an error because the build system doesn't kow yet what is the
target platform. We will fix this using ``ccmake``, a console tool used to
configure cmake cache:

.. code-block:: console

    [root@6bf01305461f build]# ccmake .


ccmake only displays one configuration variable, LZ_TARGET_MACHINE_CHOICE,
by hitting ENTER (RETURN) will cycle through the supported platforms.

.. code-block:: console

                                                        Page 1 of 1
    LZ_TARGET_MACHINE_CHOICE

















    LZ_TARGET_MACHINE_CHOICE: Choice of the target machine.                         
    Keys: [enter] Edit an entry [d] Delete an entry             CMake Version 3.17.2
        [l] Show log output   [c] Configure
        [h] Help              [q] Quit without generating
        [t] Toggle advanced mode (currently off)


For now, only AVR_ATmega328p is supported.

.. code-block:: console

                                                        Page 1 of 1
    LZ_TARGET_MACHINE_CHOICE         AVR_ATmega328p

















    LZ_TARGET_MACHINE_CHOICE: Choice of the target machine.                         
    Keys: [enter] Edit an entry [d] Delete an entry             CMake Version 3.17.2
        [l] Show log output   [c] Configure
        [h] Help              [q] Quit without generating
        [t] Toggle advanced mode (currently off)


We can now ask CMake to configure the cross-compilation toolchain, hitting ``c``:

.. code-block:: console


    The C compiler identification is GNU 9.2.0
    The ASM compiler identification is GNU
    Found assembler: /usr/bin/avr-gcc
    Check for working C compiler: /usr/bin/avr-gcc
    Check for working C compiler: /usr/bin/avr-gcc - works
    Detecting C compiler ABI info
    Detecting C compiler ABI info - done
    Detecting C compile features
    Detecting C compile features - done
    Using module: module_version_string
    Configuring done








    Configure produced the following output                                         
                                                                CMake Version 3.17.2
    Press [e] to exit screen



After exiting this screen by hitting ``e``, ccmake now displays all the
available configuration variables for the toolchain it created :

.. code-block:: console

                                                        Page 1 of 3
    AVR_AR                          */usr/bin/avr-ar
    AVR_CC                          */usr/bin/avr-gcc
    AVR_NM                          */usr/bin/avr-nm
    AVR_OBJCOPY                     */usr/bin/avr-objcopy
    AVR_OBJDUMP                     */usr/bin/avr-objdump
    CMAKE_BUILD_TYPE                *
    CMAKE_INSTALL_PREFIX            */usr/local
    CMAKE_TOOLCHAIN_FILE            */~/workspace/src/cmake/avr.toolchain.cmake   
    LZ_CONFIG_AVR_INSTRUMENT_PORT   *0x2B
    LZ_CONFIG_BREAK_STACK_GAP       *50
    LZ_CONFIG_CHECK_INTERRUPT_CODE  *ON
    LZ_CONFIG_CHECK_NULL_PARAMETER  *ON
    LZ_CONFIG_CHECK_NULL_PARAMETER  *ON
    LZ_CONFIG_CHECK_NULL_PARAMETER  *ON
    LZ_CONFIG_CHECK_NULL_PARAMETER  *ON
    LZ_CONFIG_CHECK_WRONG_ENUM_ENT  *ON
    LZ_CONFIG_DEFAULT_TASK_PRIORIT  *0

    AVR_AR: Path to a program.                                                      
    Keys: [enter] Edit an entry [d] Delete an entry             CMake Version 3.17.2
        [l] Show log output   [c] Configure
        [h] Help              [q] Quit without generating
        [t] Toggle advanced mode (currently off)


You can configure these variables according to your needs and hit ``c`` again:

.. code-block:: console

    Using module: module_version_string
    Configuring done

















    Configure produced the following output                                         
                                                                CMake Version 3.17.2
    Press [e] to exit screen

After hitting ``e``, you can now generate the full configured toolchain and exit
by hitting ``g``. You can then return to the workspace:

.. code-block:: console

    [root@6bf01305461f build]# cd ..
    [root@6bf01305461f workspace]# 


Note that if you already know your target platform,
the cross compilation toolchain can be generated by doing only:

.. code-block:: console

    [root@6bf01305461f workspace]# cmake -S . -B ./build -DLZ_TARGET_MACHINE_CHOICE=AVR_ATmega328p
    -- The C compiler identification is GNU 9.2.0
    -- The ASM compiler identification is GNU
    -- Found assembler: /usr/bin/avr-gcc
    -- Check for working C compiler: /usr/bin/avr-gcc
    -- Check for working C compiler: /usr/bin/avr-gcc - works
    -- Detecting C compiler ABI info
    -- Detecting C compiler ABI info - done
    -- Detecting C compile features
    -- Detecting C compile features - done
    Using module: module_version_string
    -- Configuring done
    -- Generating done
    -- Build files have been written to: /~/workspace/build
    [root@6bf01305461f workspace]# 


At any moment you can set configuration variables by invoking ccmake pointing
to the ``build`` directory:

.. code-block:: console

    [root@6bf01305461f workspace]# ccmake build/

Modules

Compilation
^^^^^^^^^^^

Compilation is quite straightforward. Simply invoke ``cmake`` with
``build`` option pointing to the ``build`` directory:

.. code-block:: console

    [root@6bf01305461f workspace]# cmake --build ./build
    Scanning dependencies of target module_version_string
    [  4%] Building C object src/kern/modules/version_string/CMakeFiles/module_version_string.dir/version_string.c.obj
    [  4%] Built target module_version_string
    Scanning dependencies of target LazuliKernel_AVR_ATmega328p_0.1.0
    [  9%] Building C object src/CMakeFiles/LazuliKernel_AVR_ATmega328p_0.1.0.dir/kern/arch/AVR/arch.c.obj
    [ 13%] Building ASM object src/CMakeFiles/LazuliKernel_AVR_ATmega328p_0.1.0.dir/kern/arch/AVR/interrupt_vectors_table.S.obj
    [ 18%] Building ASM object src/CMakeFiles/LazuliKernel_AVR_ATmega328p_0.1.0.dir/kern/arch/AVR/startup.S.obj
    [ 22%] Building C object src/CMakeFiles/LazuliKernel_AVR_ATmega328p_0.1.0.dir/kern/arch/AVR/timer_counter_1.c.obj
    [ 27%] Building C object src/CMakeFiles/LazuliKernel_AVR_ATmega328p_0.1.0.dir/kern/kernel.c.obj
    [ 31%] Building C object src/CMakeFiles/LazuliKernel_AVR_ATmega328p_0.1.0.dir/kern/memory.c.obj
    [ 36%] Building C object src/CMakeFiles/LazuliKernel_AVR_ATmega328p_0.1.0.dir/kern/scheduler.c.obj
    [ 40%] Building C object src/CMakeFiles/LazuliKernel_AVR_ATmega328p_0.1.0.dir/kern/list.c.obj
    [ 45%] Linking C static library libLazuliKernel_AVR_ATmega328p_0.1.0.a
    [ 45%] Built target LazuliKernel_AVR_ATmega328p_0.1.0
    Scanning dependencies of target LazuliUserProject_AVR_ATmega328p_1.0.0
    [ 50%] Building C object CMakeFiles/LazuliUserProject_AVR_ATmega328p_1.0.0.dir/user/main.c.obj
    [ 54%] Linking C executable LazuliUserProject_AVR_ATmega328p_1.0.0
    [ 54%] Built target LazuliUserProject_AVR_ATmega328p_1.0.0
    Scanning dependencies of target hex_output
    [ 59%] Generating user HEX file: LazuliUserProject_AVR_ATmega328p_1.0.0.hex
    [ 59%] Built target hex_output
    Scanning dependencies of target lst_ouput
    [ 63%] Generating user LST file: LazuliUserProject_AVR_ATmega328p_1.0.0.lst
    [ 63%] Built target lst_ouput
    Scanning dependencies of target kernel_lst_ouput
    [ 68%] Generating kernel LST file: LazuliKernel_AVR_ATmega328p_0.1.0.lst
    [ 68%] Built target kernel_lst_ouput
    Scanning dependencies of target module_clock_24
    [ 72%] Building C object src/kern/modules/clock_24/CMakeFiles/module_clock_24.dir/clock_24.c.obj
    [ 72%] Built target module_clock_24
    Scanning dependencies of target module_mutex
    [ 77%] Building C object src/kern/modules/mutex/CMakeFiles/module_mutex.dir/mutex.c.obj
    [ 81%] Building ASM object src/kern/modules/mutex/CMakeFiles/module_mutex.dir/arch/AVR/mutex.S.obj
    [ 81%] Built target module_mutex
    Scanning dependencies of target module_serial
    [ 86%] Building C object src/kern/modules/serial/CMakeFiles/module_serial.dir/serial.c.obj
    [ 90%] Building C object src/kern/modules/serial/CMakeFiles/module_serial.dir/arch/AVR/usart.c.obj
    [ 90%] Built target module_serial
    Scanning dependencies of target module_spinlock
    [ 95%] Building C object src/kern/modules/spinlock/CMakeFiles/module_spinlock.dir/spinlock.c.obj
    [100%] Building ASM object src/kern/modules/spinlock/CMakeFiles/module_spinlock.dir/arch/AVR/spinlock.S.obj
    [100%] Built target module_spinlock
    [root@6bf01305461f workspace]# 

You can notice that the build system builds all modules,
even those that are not used by your project.
However they are built *but not linked to your project*.
This is to ensure that code modifications made to the kernel doesn't break
integrity of the *whole kernel*, including modules.
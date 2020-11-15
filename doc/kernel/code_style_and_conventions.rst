..
   SPDX-License-Identifier: GPL-3.0-only
   This file is part of Lazuli.

.. sectionauthor::
   Copyright (c) 2020, Remi Andruccioli <remi.andruccioli@gmail.com>

Code style and conventions
==========================

..
    TODO: Use the templates.

C style and conventions
-----------------------

We explain here the programming style and conventions to apply on C code for the
project.

As a general rule, the code must be clear and readable. Reading it must be a
pleasant experience.
The code must not contain useless or redundant things.

C dialect
^^^^^^^^^

The whole project is written in pure ANSI C 89.
All C code files are encoded in raw 7-bit ASCII.

.. note::
   This is to the allow C code to be ported easily to many platforms and
   architectures, by being compiled by the largest number of compilers.
   Another reason is that C89 is understood by all C code-checking tools.

Whitespaces, tabs and newlines
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Never use tabs to indent C code. C code must use 2 spaces for indentation.

.. note::
   Every editor must display the code the same way.


Lines mustn't have trailing whitespaces.

Files must end with a newline character.

.. note::
   The code file must print correctly when using tools like ``cat`` or ``less``
   in the console.

..
    TODO: spaces before () in if, for, while

Line length
^^^^^^^^^^^

Line length is set to 80 characters. Each line of code shouldn't exceed 80
columns.

.. note::
   Documents must be easily readable without horizontal scrolling in a console
   (e.g. with ``less`` or ``cat``), in simple text editors, or when comparing
   two versions in a file diff utility.

Comments
^^^^^^^^

Only use C89 comments, no C99/C++ comments.

.. note::
   This is to be compliant with C89.

Never write nested comments. Never write "pretty typos".
Never comment out code unless you want to document something.

Use only one of the 2 following forms.
1-line comments can use both allowed forms, while multiple line comments must
use the second form only.

This is good:

.. code-block:: c

   /* First form. This is a comment */

   /*
    * Second form.
    *
    * This is a comment
    * that spans over
    * multiple lines.
    */

   /*
    * The second form can be used for 1-line comments.
    */

This is *not* good:

.. code-block:: c

   // This is a comment.

   /* This is a comment
      that spans over
      multiple lines */

    /***************************************************
     *             Wonderful pretty typo               *
     ***************************************************/

Where to put comments
*********************

Good code shouldn't need comments.
Good code could should express by itself.

As a general rule:

* Always comment the API of every function, even static ones
* Avoid comments inside functions

Use comments inside functions very carefully only to explain something that is
not obvious for the reader.

Every function must be documented using Doxygen tags.
For static functions, this is done above the function itself.
For public functions, this is done above the function prototype declaration
in the header file only.

Return types
^^^^^^^^^^^^

The return type of a function must be declared on the line preceding the
function name.

This is good :

.. code-block:: c

   int
   sum(int a, int b)
   {
     return a + b;
   }

This is *not* good :

.. code-block:: c

   int sum(int a, int b)
   {
     return a + b;
   }

.. note::
   C code must be easily readable in console or with editors that don't support
   syntax highlighting.


Curly braces
^^^^^^^^^^^^

Curly braces use the K&R style.

For functions, opening and closing braces are always on their own line.

This is the only style allowed for functions:

.. code-block:: c

    int SayHello(void)
    {
      printf("Hello world\n");

      return 0;
    }

For control statements the opening braces are always on the same line than the
statement and the closing braces are always on a new line.

This is the only style allowed for control statements:

.. code-block:: c

    if (n == 4) {
      printf("Bye");

      return 0;
    }

This applies to all control statements that involve code sections:
``if``, ``else``, ``for``, ``while``, ``do``.

For if-else blocks, the else keyword must be on the same line than the braces.

This is the only style allowed for if-else blocks:

.. code-block:: c

    if (n == 4) {
      j += 8;
      printf("Bye");
    } else {
      j = 0;
      i++;
    }

For control statements that involve code section, braces must *always* be
present, even for one-line expressions:

.. code-block:: c

    if (NULL == map) {
      return NULL;
    }

    for (i = 0; i < LENGTH; i++) {
      t[i] = NULL;
    }

    if (NULL == map) {
      return NULL;
    } else {
      return map->next;
    }

.. note::
   Always putting the braces helps to avoid some bugs.
   As an example, you can read this article about the famous case of Apple's
   SSL/TLS bug:
   `<https://embeddedgurus.com/barr-code/2014/03/apples-gotofail-ssl-security-bug-was-easily-preventable/>`_

..
    TODO:
    one-line ifs
    Naming
    Casing

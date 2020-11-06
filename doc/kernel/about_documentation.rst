..
   SPDX-License-Identifier: GPL-3.0-only
   This file is part of Lazuli.

.. sectionauthor::
   Copyright (c) 2016-2020, Remi Andruccioli <remi.andruccioli@gmail.com>

About documentation
===================

The documentation for the projects comes from 2 different sources:

* **Textual documentation**, generated with **Sphinx** from text files written
  in rst format.
* **API documentation**, generated with **Doxygen** from formatted code
  comments.

The documentation is rendered in 2 formats:

* **HTML**
* **man**: The man page output is used in the Lazuli Docker image.
  Any developer using the development environment Docker image can type
  ``man Lazuli`` to read the exact complete documentation you are now reading.
  Thanks to Doxygen, the user can also use ``man`` to obtain the documentation
  about any function or symbol used in the project.

Sphinx and reStructuredText
---------------------------

The directory ``doc/`` in the repository source tree contains all the official
documentation of the project. This is the one you are now reading.

The Lazuli project aims to be well-documented. Documentation must be up-to-date
and easy to find.

This documentation is written in reStructuredText (rst) and built with Sphinx.
The official website of sphinx is `<https://www.sphinx-doc.org>`_

reStructuredText file markup allows to write documents that can be read in text
mode within a console as well as in a modern browser, while benefiting of a nice
formatting on the two supports.

Building Sphinx documentation
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

To build Sphinx documentation, simply go to the ``doc/`` directory, then type
``make``. This will build the full documentation in HTML and man formats.
The output will be in the directory ``doc/_build``.

To build the documentation only in a specific format, simply type ``make html``
or ``make man``.


Style and conventions used in documentation files
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Files must be named using lower camel case with underscores as separators and
have the extension ``.rst``
(e.g. the file your are reading is called ``about_documentation.rst``).

Except in some very specific situations (e.g. when writing URLs), line length is
set to 80 characters.
Documents must be easily readable without horizontal scrolling in a console
(e.g. with ``less``), in simple text editors, in a file diff utility, or on any
platform that don't natively supports rst syntax.
80 columns line breaks are not a problem when rendering to HTML or man pages as
rst ignores line breaks when processing.

Always keep in mind that the file can be read in one of these two possible ways,
so you should regularly check how it looks on both supports when writing.

Headings are always declared with underlining.
Heading characters are used in the following order:

#. ``=``
#. ``-``
#. ``^``

There must be only one heading of level 1 per page.

Doxygen
-------

The other kind of documentation comes from code comments with Doxygen commands.
The official website of Doxygen is `<https://www.doxygen.nl>`_

The API documentation for the project is hosted at
`<https://randruc.github.io/Lazuli/doxygen/latest/>`_

Code comments
^^^^^^^^^^^^^

All C code of the project is documented using Doxygen code comments.
The absence of Doxygen code comments will cause an error during build.

Building Doxygen documentation
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

To build Doxygen documentation, simply go to the root of the repository, then
type ``doxygen``. This will build documentation in HTML and man formats.
The output will be in the directory ``doxygen_output``.

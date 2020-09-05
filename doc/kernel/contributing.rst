..
   SPDX-License-Identifier: GPL-3.0-only
   This file is part of Lazuli.

.. sectionauthor::
   Copyright (c) 2020, Remi Andruccioli <remi.andruccioli@gmail.com>

Contributing
============

**Contributions are welcome!**

*However, contributions must follow a set of rules.*

It doesn't matter how big your contribution is: it can be fixing a bug,
fixing a spelling mistake or a typo, suggesting new features, or implementing
new functionalities to the kernel.
Any kind of contribution is welcome!

Contribute without coding
-------------------------

You can use the ``Issues`` tab (`<https://github.com/randruc/Lazuli/issues>`_)
of the project to report troubleshooting, spelling mistakes or typos, ask
questions, suggest new features, or anything you have in mind!

Contribute to the code
----------------------

The official code repository for the project is on GitHub at
`<https://github.com/randruc/Lazuli>`_. GitHub is used to manage all the
project: code, pull requests, issues, etc.

Before contributing, make sure you've read :

* The page :doc:`about_documentation` if you wish to contribute to the
  documentation.
* The page :doc:`code_style_and_conventions` if you wish to contribute to the
  code.

When coding, make sure you've updated all the appropriate files and lines of
text apart from the code itself. That includes code comments, documentation
files, CMakeLists.txt, or any other file that could be impacted by your change.

Contributions is done using the classic GitHub pull-request mechanism:

* Fork the repository.
* Create a branch *with a meaningful name* that will bear your contribution.
* Commit your contribution on your branch. Be careful to respect to expected
  format for commit messages. Read below for the detail.
* Push your branch, then do the pull request. If your contribution is not
  obvious, it is recommended to explain it with a message.
* Your contribution will then be reviewed. Be aware that questions can be asked
  or remarks can be made before the ultimate merging.

For now, squashing and rebasing are disabled for pull-requests.
  
Commit messages
^^^^^^^^^^^^^^^

The expected format for commit messages is the following:

.. code-block:: none

   <Part or module>: <Brief explanation on one line.>

   <Detailled explanation if needed.>
   <The detailled explanation can be expressed as a list.>

Here are a few examples of real commit messages that follow this format:

(from commit 5cca0fd69947409221b6337c98c6b1e3ac9419ef)

.. code-block:: none

   GitHub Actions: Set the "pipefail" bash option when building docker image.
    
   This is needed because we pipe the output of "docker build" to "ts". So if
   "docker build" fails, the exit code is not the one of "ts".


(from commit d31a215cf821b84804cbadf5169711d6c4ccc6a9)

.. code-block:: none

   stack_usage.sh: Improvements on the usability.
    
   * Command line options have been added to let the user sort by stack size or
     by file name.
   * The output is no longer piped into less.

Continuous Integration
^^^^^^^^^^^^^^^^^^^^^^

Continuous Integration (CI) is configured on the official GitHub repository
using GitHub Actions, and is triggered on pull-requests.

The project's CI performs the following tasks:

* Execute ``scripts/checklines.sh`` to check for trailing whistespaces, line
  length, etc.
* Build the Lazuli development environment Docker image from the root
  ``Dockerfile``.
* In the newly built Docker image, configure then build Lazuli in its default
  configuration (reminder: modules are always built even if not selected to be
  linked to the final binary).
* Build Doxygen documentation.
* Build Sphinx documentation.

All the CI pipeline is configured to treat warnings are errors. A pull-request
with a failing CI has no chance to be merged.

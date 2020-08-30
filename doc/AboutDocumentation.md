<!---
SPDX-License-Identifier: GPL-3.0-only
This file is part of Lazuli.
Copyright (c) 2016-2020, Remi Andruccioli <remi.andruccioli@gmail.com>
--->

# About documentation

The directory `doc/` in the Lazuli source tree contains all the documentation
about the project.

The Lazuli project aims to be well-documented. Documentation must be up-to-date
and easy to find.

## Mardown

This documentation is written in Markdown. It allows to write documents that
can be read in text mode within a console as well as in a modern browser, while
benefiting of a nice formatting on the two supports.

The reference for the Mardown syntax can be found here:
https://daringfireball.net/projects/markdown/syntax

For emacs the package `markdown-mode` (melpa) is very useful.

## Writing documents

### Style and conventions used in document files

Files must be named using UpperCamelCase (as in `AboutDocumentation.md`).

When writing a document each line must not exceed 80 columns in length.
Documents must be easily readable without scrolling in a console (e.g. with
`less`), in simple text editors, in a file diff utility, or with browsers that
doesn't support the Markdown syntax.  
This limit isn't a problem when viewing the file in a modern browser since
Markdown ignores simple line breaks.

Always keep in mind that the file will be read in one of these two possible
ways, so you should regularly check how it looks on both supports when writing.

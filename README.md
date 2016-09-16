# Shredder by Piotr Jander


## Your name and eniac username

Piotr Jander, piotr


## A list of submitted source files

``
- main.c
- Makefile
- minunit.h
    - 3rd party minimal testing utilities
- parse.c, parse.h
- README.md
- tests.c
    - includes functions from parse.c
    - has its own main() function which needs uncommenting
    - file attached for reference only
``


## Extra credit answers

- Implemented `atoi`.
- Shell commands can take arguments. I implemented parsing.
- Questions about signals NOT answered.


## Compilation Instructions

`make`


## Overview of work accomplished

I believe my shell meets all requirements. I now better understand
system calls, especially `read`/`write` and buffers.

I also came up with a reasonably effective C development workflow:
 program modularisation, a build system, unit testing, debugging,
 writing the code on Mac and running on a virtual machine, etc.


## Description of code and code layout

The layout of `main.c` roughly follows hints given in the assignment.

To separate error handling in system calls from the program's logic,
each system call has a corresponding wrapper function, defined
at the bottom of `main.c`.

Functions which do command arguments parsing are in `parse.c`. This module
has one public function, `parse_input`, defined
in the corresponding header file.


## General comments and anything that can help us grade your code

I hope the code will explain the rest!

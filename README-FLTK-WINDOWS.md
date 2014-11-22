Intro
=====

This is a procedure of how to use and build
the fltk libraries on a Windows machine so
it can be used in CodeBlocks.

Contents
========

- Downlaod fltk
- Download cmake
- Download CodeBlocks
- Generate fltk makefiles
- Setup Code::Blocks


fltk
====

- Download the fltk source, e.g. version 1.3.3 from http://www.fltk.org/
- Unzip the tarball, e.g. to x:\

Items in directory x:\fltk-1.3.3

cmake
=====
- Download cmake binary for Windows, e.g. version 3.0.2 (not 3.1.0 rc2!!) from http://www.cmake.org/
- Unzip the binary, e.g. to x:\

Items in directory x:\cmake-3.0.2-win32-x86

CodeBlocks
==========
- Download Code::Blocks plus minGW32 setup for Windows version 13.12 from http://www.codeblocks.org/
- Install Code::Blocks, e.g. to c:\Program Files (x86)\CodeBlocks
- Start C:\Program Files (x86)\CodeBlocks\MinGW\mingwvars.bat to set the environement variable PATH

Generate fltk makefiles
=======================
- Start x:\cmake-3.0.2-win32-x86\bin\cmake-gui.exe
- Set browse source to x:\fltk-1.3.3
- Set build path to x:\fltk-1.3.3\lib
- Click configure
- (first time) select "specify native compilers", click next
- Set C to C:/Program Files (x86)/CodeBlocks/MinGW/bin/gcc.exe
- Set C++ to C:/Program Files (x86)/CodeBlocks/MinGW/bin/g++.exe, click finish

configuration is now busy!

- Click configure again , all red background will disappear
- Click generate
- Close cmake-gui

Now the makefile for minGW are generated in x:\fltk-1.3.3\lib

Build fltk libraries
====================

- Open command shell as administrator
- Change directory to x:\fltk-1.3.3\lib
- Enter minGW32-make install

This takes some time. The fltk libraries are compiled and
will be linked into C:\Program Files (x86)\FLTK

Setup CodeBlocks
================

- Set search directory for compiler to C:\Program Files (x86)\FLTK
- Set search directory for linker to C:\Program Files (x86)\FLTK and C:\Program Files (x86)\FLTK\lib
- If creating a new fltk project remove subdirectory FL\ from includes.

#include <FL\Fl.H>  becomes #include <Fl.H>



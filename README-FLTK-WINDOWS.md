Intro
=====

This is a procedure of how to use and build
the fltk libraries on a Windows8 and XP machine so
it can be used in CodeBlocks with MinGW.

Contents
========

- Download fltk
- Download cmake
- Download CodeBlocks+MinGW
- Generate fltk makefiles
- Setup Code::Blocks


fltk
====

- Download the fltk source, e.g. version 1.3.3 from http://www.fltk.org/
- Unzip the tarball, e.g. to x:\

Items in directory x:\fltk-1.3.3

cmake
=====
- Download cmake binary for Windows, e.g. version 3.0.2 from http://www.cmake.org/
- Unzip the binary, e.g. to x:\

Items in directory x:\cmake-3.0.2-win32-x86

CodeBlocks
==========
- Download Code::Blocks plus minGW32 setup for Windows (version 13.12) from http://www.codeblocks.org/
- Install Code::Blocks, e.g. to c:\Program Files (x86)\CodeBlocks
- Start C:\Program Files (x86)\CodeBlocks\MinGW\mingwvars.bat to set the environement variable PATH

Generate fltk makefiles
=======================
- Start x:\cmake-3.0.2-win32-x86\bin\cmake-gui.exe
- Set browse source to x:\fltk-1.3.3
- Set build path to x:\fltk_makefiles
- Click configure
- (first time) select "specify native compilers", click next
- Set C to C:/Program Files (x86)/CodeBlocks/MinGW/bin/gcc.exe
- Set C++ to C:/Program Files (x86)/CodeBlocks/MinGW/bin/g++.exe, click finish

configuration is now busy!

- Click configure again , all red background will disappear
- Click generate
- Close cmake-gui

Now the makefiles for minGW are generated in x:\fltk_makefiles

Build fltk libraries
====================

- Open command shell as administrator
- Change directory to x:\fltk_makefiles
- Enter minGW32-make install

This takes some time. The fltk libraries are compiled and
will be default linked into C:\Program Files (x86)\FLTK on a
64bit Windows machine and C:\Program Files\FLTK on a 32bit Windows machine.

Setup CodeBlocks
================
The Codeblocks fltk wizard.script set search directories to
 "include" and "lib" subdirectories. The former isn't part
of the unziped fltk-1.3.3 but can be found in C:\Program Files (x86)\FLTK.
 
snippet of fltk wizard.script
// globals (windows only)
FltkPathDefault    <- _T("$(#fl)");
FltkPathDefaultInc <- _T("$(#fl.include)");
FltkPathDefaultLib <- _T("$(#fl.lib)");


In Codeblocks 
- Open settings\Global Variables
- Select current variable to "fl"
- Set base to C:\Program Files (x86)\FLTK or C:\Program Files\FLTK and close the Global Variable Editor

Next...
- Create a new FLTK project e.g. "whataplumbing"  :P
- Build and try
But...
- In Windows XP project file uses global variable instead of C:\Program Files (x86)\FLTK, so portable.
- In Windows 8 project file uses C:\Program Files (x86)\FLTK instead of global variable, so not portable.






# XMAS Windows C++ IDE Setup

### Preconditions
  - [fltk] libraries version 1.3.3 installed.
  - [CodeBlocks] version 13.12 with minGW32 installed.

In Windows the fltk libraries default build location is the *ProgramFiles* folder.
On a 64bit system the default *ProgramFiles* folder its environement variable is
%ProgramFiles(x86)% and %ProgramFiles% on a 32bit system.

How fltk libraries can be build on a Windows system can be found in [README-FLTK-WINWODS].

### Setup CodeBlocks
* Start CodeBlocks
* Open *Settings* menu
* Choose *Global Variables...*
* Verify that the current variable is *fl*
* Set *base* to the FLTK build path e.g. *"C:\Program Files (x86)\FLTK"*

Now CodeBlocks has been setup to create FLTK projects. 
To create a new FLTK project, simply select the FLTK project template
when creating a new project.
A simple *"Hello world!"* application is created automatically which
can be build and run.


[fltk]:http://www.fltk.org/
[CodeBlocks]:http://www.codeblocks.org/
[README-FLTK-WINWODS](README-FLTK-WINWODS.md)


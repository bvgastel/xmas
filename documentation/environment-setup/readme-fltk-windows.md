# Setup FLTK for Windows

In this readme: How to setup FLTK on a Windows platform for GNU C++
with CodeBlocks.


### Precondition
  - MinGW installed
  - g++ can be found by the path variable.
  - A directory to store C++ tools e.g. *"c:\cpptools"*

### Build FLTK libraries

(replace * with the version postfix)

 - Download [FLTK] libraries
 - Unpack fltk* to *"c:\cpptools\fltk*"*
 - Start cmake gui
 - Set source to "c:\cpptools\fltk-1.3.3"
 - Set binaries to "c:\cpptools\cmakes\fltk"
 - Click configure and confirm to create directory
 - Specify native compiler for MinGW under CodeBlocks (...wait)
 - Change CMAKE_INSTALL_PREFIX to "c:/cpptools/fltk*"
 - Uncheck OPTION_BUILD_EXAMPLES
 - Click configure again until all red lines disappear
 - Click generate
 - Close cmake gui
 - Open command prompt as administrator
 - Enter "mingw32-make install" (...wait)
 - Close command window

 
 The fltk libraries can be found at c:\cpptools\fltk*\lib.

 
[FLTK]:http://www.fltk.org/



# Setup Boost for Windows

In this readme: How to setup Boost on a Windows platform for GNU C++
with CodeBlocks.
Detailed information van be found at [Boost Compiler Choice] and [Boost getting started for Windows]
 
### Precondition
  - MinGW installed
  - g++ can be found by the path variable.
  - A directory to store C++ tools e.g. *"c:\cpptools"*

### Build Boost libraries
Most libraries in boost are header only and do not need to be built. (see [Boost getting started for Windows])

(replace * with the version postfix)

 - Download [Boost] libraries
 - Unpack boost* to *"c:\cpptools\boost*"*
 - Open command prompt as administrator
 - Change directory to c:\cpptools\boost*\
 - Enter *"bootstrap mingw"*
 - Verify if "project-config.jam" has line *"using gcc;"*. If not modify "msvc" with "gcc".
 - Build all libraries by entering *b2*

 
>Build libraries can be found in subdirectory c:\cpptools\boost*\stage\lib.
>Library names ending with *...mt-d-1_57.a* are debug versions while
>those without a *"-d"* the release versions.

>Attention: The CMake build for boost is experimental!


##### References:
 * [Boost]
 * [Boost getting started for Windows]
 * [Boost Compiler Choice]
 * [Boost Simple Starter]

[Boost]:http://www.boost.org/
[Boost getting started for Windows]:http://www.boost.org/doc/libs/1_57_0/more/getting_started/windows.html
[Boost Compiler Choice]:http://www.boost.org/build/doc/html/bbv2/reference/tools.html
[Boost Simple Starter]:http://www.bogotobogo.com/cplusplus/boost.php


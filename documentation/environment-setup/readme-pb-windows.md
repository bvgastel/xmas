# Setup Protocol Buffers for Windows

In this readme: How to setup PB on a Windows platform for GNU C++.


### Precondition
  - MinGW installed
  - msys installed
  - g++ can be found by the path variable.
  - A directory to store C++ tools e.g. *"c:\cpptools"*

### Build PB libraries

(replace * with the version postfix)

 - Download [Protocol Buffers] libraries
 - Unpack protobuf* to *"c:\cpptools\protobuf*"*
 - Start msys bash (desktop icon)
 - Enter "cd /e/cpptools/protobuf*"
 - Enter ./configure (...wait)
 - Enter make
 
 The protobuf libraries can be found at c:\cpptools\protobuf-2.6.1\src\.libs.
 
 
[Protocol Buffers]:http://code.google.com/p/protobuf/



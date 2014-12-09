# CodeBlocks for Windows

### Preconditions
 - [readme-windows.md](/readme-windows.md) done.
 - CodeBlocks not installed.
 - MinGW not installed.
 
On a 64bit Windows system the default *ProgramFiles*
folder its environement variable is %ProgramFiles(x86)%
and %ProgramFiles% on a 32bit system.

### Install CodeBlocks

	- Download CodeBlocks
	- Start the downloaded exe

CodeBlocks is now installed in %ProgramFiles%.
	

### Setup CodeBlocks for FLTK

 - Start CodeBlocks
 - Open *Settings* menu
 - Choose *Global Variables...*
 - Add current variable *fl*
 - Set *base* of *fl* to the fltk build path *"c:\cpptools\fltk*"*

>Now CodeBlocks has been setup to create FLTK projects. 
>To create a new FLTK project, simply select the FLTK project template
>when creating a new project.
>Enter $(#fl) in "Please select FLTK's location"
>A simple *"Hello world!"* application is created automatically which
>can be build and run.
 
 
### Setup CodeBlocks for Protocol Buffers

 - Start CodeBlocks
 - Open *Settings* menu
 - Choose *Global Variables...*
 - Add current variable *protobuf*
 - Set *base* of *protobuf* to the protobuf build path *"c:\cpptools\protobuf*"*

### Setup CodeBlocks for Boost
	todo
	
[CodeBlocks]:http://www.codeblocks.org/

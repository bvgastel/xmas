# XMAS project directory structure

The source inspiration for this structure is an answer to 

<http://stackoverflow.com/questions/1417776/how-to-use-qmakes-subdirs-template> 

However, we no longer use the source provided there, because qmake-qt has
developed further. We can now use standard qmake instructions to develop
the source on windows and linux.

We adjusted the source structure to our needs. The top directory (this one) contains
only projects, no .pro file. Each project has its own main .pro file. 

The main .pro file either contains further subprojects, in which case it uses template 'subdirs',
or it is independent, in which case it uses template 'app' or 'lib'. The subprojects 
always use 'app' or 'lib' for template.

For Windows we use static linking and for Linux we use dynamic linking. Do not ask.
We found that using dynamic libraries under Windows gives "undefined" link errors, while
using static under Linux seems not to work at all: it still needs the .so files.
We all hate it when things do not work properly. We would love to have this solved
and use static on both platforms. There is no need to use dynamic as the libraries
are all tied to this project.

The current project structure is:

~~~~~~~~~~~~~~~~~~~~~~~~~~~
./bitpowder			Library 			A supporting lib
./bitpowdertest		Main: gtest 		test cases for bitpowder
./xmd				SUBDIRS				the xmas designer
./xmd/xmd			Library, Qml2		The actual qml code
./xmd/xmdmain		Main, Qt			The Qt code to start qml
./xmv 				SUBDIRS 			A supporting set of libs
./xmv/datamodel		Library 			The datamodel of xmas
./xmv/vt 			Library 			The verification tools of xmas
./xmv/xmvmain 		Main 		
./xmvtest 			Main: gtest 		test cases for xmv
~~~~~~~~~~~~~~~~~~~~~~~~~~~



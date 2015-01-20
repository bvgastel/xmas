Prepare a Linux platform
========================


Development
-----------

Install Qt5 libraries and Qt5-devel libraries.
Install QtCreator

Use your package manager to install the required software (see Fedora).

For development we use *.pro files with qmake-qt5. QtCreator works with .pro files 
natively.

After using qmake, you should be able to build, either with QtCreator or with your
default build utilities. From Linux we use make files.

Commandline testing
-------------------
Testing outside QtCreator requires you to set the variable LD_LIBRARY_PATH
if you use internal libraries in your project.

The statement 

~~~~~~~
ldd <executable>
~~~~~~~

will show you which libraries are missing. All you have to do is add the paths to those
libraries to the variable LD_LIBRARY_PATH and execute the program. Example for
a build directory where the current dir is where your executable is and
the libraries are respectively in ../test and ../model

~~~~~~~~~~
$> export LD_LIBRARY_PATH=../test:../model 
$> ./<executable>
~~~~~~~~~~

If you want to create a script for testing from the commandline, use absolute paths for the model.

REMARK. ldd will sometimes execute an executable to find the library dependancies. Do not use ldd
on executables you do not fully trust.

# XMAS project directory structure

The source inspiration for this structure is an answer to 

<http://stackoverflow.com/questions/1417776/how-to-use-qmakes-subdirs-template> 

The proposed structure adjusted to our needs. 

~~~~~~~~~~~~~~~~~~~~~~
xmas/
-xmas.pro
-common.pri
-xmas/
----xmas.pro
----some logic files fro xmas
----leads to libxmas.a (on linux)
-xmd/
----xmd.pro
----xmd files
----leads to libxmd.a (on linux)
-xmasmain/
----xmasmain.pro
----main.cpp
----leads to xmd (executable on linux)
-testmain/
----testmain.pro
----main.cpp
----leads to test (executable on linux)
~~~~~~~~~~~~~~~~~~~~~~

xmas.pro:

~~~~~~~~~~~~~~~~~~~~~~
TEMPLATE = subdirs
SUBDIRS = xmaslib \
          xmd

# xmasmain must be last:

CONFIG += ordered
SUBDIRS += xmasmain
~~~~~~~~~~~~~~~~~~~~~~

common.pri:

~~~~~~~~~~~~~~~~~~~~~~
#Includes common configuration for all subdirectory .pro files.
INCLUDEPATH += . ..
WARNINGS += -Wall

TEMPLATE = lib

# The following keeps the generated files at least somewhat separate 
# from the source files.
UI_DIR = uics
MOC_DIR = mocs
OBJECTS_DIR = objs
~~~~~~~~~~~~~~~~~~~~~~

xmas/xmas.pro:

~~~~~~~~~~~~~~~~~~~~~~
! include( ../common.pri ) {
    error( Could not find the common.pri file! )
}

HEADERS += xmas.h
SOURCES += xmas.cpp

# By default, TARGET is the same as the directory, so it will make 
# libxmas.a (in linux).  Uncomment to override.
# TARGET = target
~~~~~~~~~~~~~~~~~~~~~~

xmd/xmd.pro:

~~~~~~~~~~~~~~~~~~~~~~
! include( ../common.pri ) {
    error( Could not find the common.pri file! )
}

FORMS += xmd.ui
HEADERS += xmd.h
SOURCES += xmd.cpp

# By default, TARGET is the same as the directory, so it will make 
# libxmd.a (in linux).  Uncomment to override.
# TARGET = target
~~~~~~~~~~~~~~~~~~~~~~

xmasmain/xmasmain.pro:

~~~~~~~~~~~~~~~~~~~~~~
TEMPLATE = app

SOURCES += main.cpp

LIBS += -L../xmas -L../xmd -lxmas -lxmd

# Will build the final executable in the build directory.
TARGET = xmd
~~~~~~~~~~~~~~~~~~~~~~

testmain/testmain.pro:

~~~~~~~~~~~~~~~~~~~~~~
TEMPLATE = app

SOURCES += main.cpp

LIBS += -L../xmas -L../xmd -L../test -lxmas -lxmd -ltest

# Will build the final executable in the main project directory.
TARGET = test
~~~~~~~~~~~~~~~~~~~~~~


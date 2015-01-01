# testmain.pro
# Contains the main program for the test programs of xmas

! include( ../common.pri ) {
	error( Could not find the common.pri file )
}

TEMPLATE = app

SOURCES += main.cpp

LIBS += -L../xmas -L../xmd -L../test -L../xmc -lxmas -lxmd -lxmc -ltest

# We will build the final executable in the build directory.

TARGET = test


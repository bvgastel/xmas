# testmain.pro
# Contains the main program for the test programs of xmas

include (../xmas/defines.pri)
include (../xmd/defines.pri)
include (../xmc/defines.pri)
include (../test/defines.pri)

QT       += core
QT      += testlib

CONFIG += C++11
CONFIG += console

TEMPLATE = app

SOURCES += main.cpp

LIBS += -L../xmas -L../xmd -L../xmc -L../test -lxmas -lxmd -ltest -lxmc

# We will build the final executable in the build directory.

# TARGET = test



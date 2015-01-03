# xmasmain.pro
# Contains the main program for xmas

CONFIG += C++11

QT += widgets svg

TEMPLATE = app

include (../xmas/defines.pri)
include (../xmd/defines.pri)

SOURCES += main.cpp

LIBS += -L../xmas -L../xmd -lxmas -lxmd

# We will build the final executable in the build directory.

TARGET = xmd



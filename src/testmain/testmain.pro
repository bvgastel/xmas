# testmain.pro
# Contains the main program for the test programs of xmas

include (../xmas/defines.pri)
include (../xmd/defines.pri)
include (../xmc/defines.pri)
include (../test/defines.pri)

QT       += core
QT       += testlib

CONFIG += C++11
CONFIG += console

TEMPLATE = app

SOURCES += main.cpp

win32:CONFIG(release, debug|release): LIBS += -L../xmas/release -L../xmd/release -L../test/release -lxmas -lxmd -ltest
else:win32:CONFIG(debug, debug|release): LIBS += -L../xmas/debug -L../xmd/debug -L../test/debug -lxmas -lxmd -ltest
else:unix: LIBS += -L../xmas -L../xmd -L../test -lxmas -lxmd -ltest


# We will build the final executable in the build directory.

# TARGET = test



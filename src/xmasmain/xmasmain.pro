# xmasmain.pro
# Contains the main program for xmas

CONFIG += C++11

QT += widgets svg declarative

TEMPLATE = app

include (../xmas/defines.pri)
include (../xmd/defines.pri)

SOURCES += main.cpp

#LIBS += -L../xmas -L../xmd -lxmas -lxmd

win32:CONFIG(release, debug|release): LIBS += -L../xmas/release -L../xmd/release -lxmas -lxmd
else:win32:CONFIG(debug, debug|release): LIBS += -L../xmas/debug -L../xmd/debug -lxmas -lxmd
else:unix: LIBS += -L../xmas -L../xmd -lxmas -lxmd

# We will build the final executable in the build directory.

TARGET = xmd



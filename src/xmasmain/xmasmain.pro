# xmasmain.pro
# Contains the main program for xmas

TEMPLATE = app

SOURCES += main.cpp

LIBS += -L../xmas -L../xmd -lxmas -lxmd

# We will build the final executable in the build directory.

TARGET = xmd



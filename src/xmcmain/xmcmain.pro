# xmcmain.pro
# Contains the main program for xmc

CONFIG += C++11

TEMPLATE = app

include (../xmas/defines.pri)
include (../xmd/defines.pri)
include (../xmc/defines.pri)

QT       += core widgets

QT       -= gui

TARGET = xmc
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
	main.cpp

HEADERS += \


unix {
	target.path = /usr/lib
	INSTALLS += target
        LIBS += -L../xmas -L../xmd  -L../xmc
}

win32 {
	target.path = xmc
	INSTALLS += target
        CONFIG(release, debug|release): LIBS += -L../xmas/release -L../xmd/release  -L../xmc/release
        else: CONFIG(debug, debug|release): LIBS += -L../xmas/debug -L../xmas/debug -L../xmc/debug
}

LIBS += -L../xmc/debug -lxmc


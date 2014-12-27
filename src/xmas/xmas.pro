#-------------------------------------------------
#
# Project created by QtCreator 2014-12-27T16:51:57
#
#-------------------------------------------------

QT       -= gui

TARGET = xmas
TEMPLATE = lib

DEFINES += XMAS_LIBRARY

SOURCES += component.cpp \
    port.cpp \
    noc.cpp

HEADERS += component.h\
        xmas_global.h \
    port.h \
    noc.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

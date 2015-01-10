#-------------------------------------------------
#
# Project created by QtCreator 2014-12-27T16:51:57
#
#-------------------------------------------------

! include( ../common.pri ) {
        error( Could not find the common.pri file)
}

include (defines.pri)

HEADERS += component.h\
        xmas_global.h \
    noc.h \
    channel.h \
    composite.h \
    primitive.h

SOURCES += component.cpp \
    noc.cpp \
    channel.cpp \
    composite.cpp \
    primitive.cpp

CONFIG += C++11
CONFIG += staticlib

TEMPLATE = lib

QT       -= gui

DEFINES += XMAS_LIBRARY

# By default TARGET is the same as the directory,
# so it will make libxmas.a (in linux). Uncomment to override.
# TARGET = target

unix {
    target.path = /usr/lib
    INSTALLS += target
}


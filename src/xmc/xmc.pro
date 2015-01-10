#-------------------------------------------------
#
# Project created by QtCreator 2014-12-23T15:40:58
#
#-------------------------------------------------

! include( ../common.pri) {
	error( Could not find the common.pri file)
}

include (defines.pri)

QT       += core widgets

QT       -= gui

TARGET = xmc
CONFIG   += console
CONFIG   += staticlib
CONFIG   -= app_bundle

TEMPLATE = lib

SOURCES += \
    control.cpp \
    consolereader.cpp \
    designer.cpp

HEADERS += \
    setup.h \
    control.h \
    consolereader.h \
    designer.h

unix {
	target.path = /usr/lib
	INSTALLS += target
}

win32 {
	target.path = xmc
	INSTALLS += target
}



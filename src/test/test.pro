#-------------------------------------------------
#
# Project created by QtCreator 2014-12-30T16:16:10
#
#-------------------------------------------------

! include( ../common.pri ) {
	error( Could not find the common.pri file )
}

QT       += core
QT      += testlib
QT       -= gui

CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

HEADERS += \
    testrunner.h

# By default TARGET is the same as the directory,
# so it will make test (in linux). Uncomment to override.
# TARGET = target

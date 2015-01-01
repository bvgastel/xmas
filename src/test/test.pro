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

CONFIG   += testcase
CONFIG   += console
CONFIG   += C++11

QT       -= gui
CONFIG   -= app_bundle

SOURCES += \
    testnoc.cpp \
    main.cpp

HEADERS += \
    testrunner.h \
    testnoc.h

# By default TARGET is the same as the directory,
# so it will make test (in linux). Uncomment to override.
# TARGET = target

unix {
	target.path = /usr/lib
	INSTALLS += target
}

#-------------------------------------------------
#
# Project created by QtCreator 2015-02-22T19:47:15
#
#-------------------------------------------------

TEMPLATE = lib

WARNINGS += -Wall

QT       += widgets
QT       -= gui

CONFIG += C++11
CONFIG += create_prl

CONFIG += dll

TARGET = utils

SOURCES += \
    result.cpp \

HEADERS +=\
    result.h \

unix|win32 {
    target.path = $$PWD/../../lib/utils
    INSTALLS += target

    headerfiles.path=$$PWD/../../include/utils
    headerfiles.files = $$PWD/*.h
    INSTALLS += headerfiles
}


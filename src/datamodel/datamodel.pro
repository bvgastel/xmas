#-------------------------------------------------
#
# Project created by QtCreator 2015-01-26T14:57:19
#
#-------------------------------------------------

QT       -= core gui

TARGET = datamodel
TEMPLATE = lib

DEFINES += DATAMODEL_LIBRARY

SOURCES += datamodel.cpp \
    xmas.cpp

HEADERS += datamodel.h\
        datamodel_global.h \
    xmas.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

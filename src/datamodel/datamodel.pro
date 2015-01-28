#-------------------------------------------------
#
# Project created by QtCreator 2015-01-26T14:57:19
#
#-------------------------------------------------

QT       -= core gui

TARGET = datamodel
TEMPLATE = lib

CONFIG += C++11

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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../bitpowder/release/ -lbitpowder
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../bitpowder/debug/ -lbitpowder
else:unix: LIBS += -L$$OUT_PWD/../bitpowder/ -lbitpowder

INCLUDEPATH += $$PWD/../bitpowder
DEPENDPATH += $$PWD/../bitpowder

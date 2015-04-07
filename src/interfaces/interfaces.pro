#-------------------------------------------------
#
# Project created by QtCreator 2015-04-06T07:41:19
#
#-------------------------------------------------

QT       += widgets

QT       -= gui

TARGET = interfaces
TEMPLATE = lib

WARNINGS += -Wall
CONFIG += C++11
CONFIG += create_prl
CONFIG += link_prl
CONFIG += dll

DEFINES += INTERFACES_LIBRARY

SOURCES += \
    logger.cpp \
    result.cpp \
    workerinterface.cpp

HEADERS += \
    commoninterface.h \
    logger.h \
    loggerinterface.h \
    result.h \
    resultinterface.h \
    vtplugininterface.h \
    workerinterface.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    readme.md

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../bitpowder/release/ -lbitpowder
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../bitpowder/debug/ -lbitpowder
else:unix: LIBS += -L$$OUT_PWD/../bitpowder/ -lbitpowder

INCLUDEPATH += $$PWD/../bitpowder
DEPENDPATH += $$PWD/../bitpowder

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../xmv/datamodel/release/ -ldatamodel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../xmv/datamodel/debug/ -ldatamodel
else:unix: LIBS += -L$$OUT_PWD/../xmv/datamodel/ -ldatamodel

INCLUDEPATH += $$PWD/../xmv/datamodel
DEPENDPATH += $$PWD/../xmv/datamodel

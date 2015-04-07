#-------------------------------------------------
#
# Project created by QtCreator 2015-04-06T08:01:11
#
#-------------------------------------------------

QT       -= gui

TARGET = vt
TEMPLATE = lib

WARNINGS += -Wall
CONFIG += C++11
CONFIG += create_prl
CONFIG += link_prl
win32: CONFIG += static
unix: CONFIG += static dll

DEFINES += VT_LIBRARY

SOURCES += \
    cycle.cpp \
    deadlock.cpp \
    vt.cpp

HEADERS += \
    cycle.h \
    deadlock.h \
    vt.h \
    vt_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    readme.md

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../bitpowder/release/ -lbitpowder
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../bitpowder/debug/ -lbitpowder
else:unix: LIBS += -L$$OUT_PWD/../../bitpowder/ -lbitpowder

INCLUDEPATH += $$PWD/../../bitpowder
DEPENDPATH += $$PWD/../../bitpowder

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../datamodel/release/ -ldatamodel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../datamodel/debug/ -ldatamodel
else:unix: LIBS += -L$$OUT_PWD/../datamodel/ -ldatamodel

INCLUDEPATH += $$PWD/../datamodel
DEPENDPATH += $$PWD/../datamodel

#-------------------------------------------------
#
# Project created by QtCreator 2015-04-06T08:22:37
#
#-------------------------------------------------

QT       += core

QT       -= gui qt

TARGET = xmvmain
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += C++11
#CONFIG += link_prl

TEMPLATE = app

WARNINGS += -Wall

SOURCES += main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../bitpowder/release/ -lbitpowder
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../bitpowder/debug/ -lbitpowder
else:unix: LIBS += -L$$OUT_PWD/../../bitpowder/ -lbitpowder

INCLUDEPATH += $$PWD/../../bitpowder
DEPENDPATH += $$PWD/../../bitpowder

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../vt/release/ -lvt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../vt/debug/ -lvt
else:unix: LIBS += -L$$OUT_PWD/../vt/ -lvt

INCLUDEPATH += $$PWD/../vt
DEPENDPATH += $$PWD/../vt

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../datamodel/release/ -ldatamodel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../datamodel/debug/ -ldatamodel
else:unix: LIBS += -L$$OUT_PWD/../datamodel/ -ldatamodel

INCLUDEPATH += $$PWD/../datamodel
DEPENDPATH += $$PWD/../datamodel

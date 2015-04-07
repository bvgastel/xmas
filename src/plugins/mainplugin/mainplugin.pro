#-------------------------------------------------
#
# Project created by QtCreator 2015-04-06T08:44:17
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = mainplugin
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += C++11

TEMPLATE = app

WARNINGS += -Wall

SOURCES += main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../bitpowder/release/ -lbitpowder
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../bitpowder/debug/ -lbitpowder
else:unix: LIBS += -L$$OUT_PWD/../../bitpowder/ -lbitpowder

INCLUDEPATH += $$PWD/../../bitpowder
DEPENDPATH += $$PWD/../../bitpowder

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../xmv/vt/release/ -lvt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../xmv/vt/debug/ -lvt
else:unix: LIBS += -L$$OUT_PWD/../../xmv/vt/ -lvt

INCLUDEPATH += $$PWD/../../xmv/vt
DEPENDPATH += $$PWD/../../xmv/vt

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../xmv/datamodel/release/ -ldatamodel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../xmv/datamodel/debug/ -ldatamodel
else:unix: LIBS += -L$$OUT_PWD/../../xmv/datamodel/ -ldatamodel

INCLUDEPATH += $$PWD/../../xmv/datamodel
DEPENDPATH += $$PWD/../../xmv/datamodel

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../interfaces/release/ -linterfaces
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../interfaces/debug/ -linterfaces
else:unix: LIBS += -L$$OUT_PWD/../../interfaces/ -linterfaces

INCLUDEPATH += $$PWD/../../interfaces
DEPENDPATH += $$PWD/../../interfaces

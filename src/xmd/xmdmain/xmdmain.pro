#-------------------------------------------------
#
# Project created by QtCreator 2015-04-06T09:04:06
#
#-------------------------------------------------

QT       += core
QT      += widgets
QT += svg
QT += quick
QT += qml
QT += quickwidgets

TARGET = xmdmain
CONFIG   += console
#CONFIG   -= app_bundle
CONFIG += C++11
CONFIG += link_prl
win32: CONFIG += static
unix: CONFIG += dll

TEMPLATE = app

# Application icon
win32|unix: RC_ICONS = app.ico
macx: ICON = app.icns

SOURCES += main.cpp

################################################
# INSTALL instructions
################################################
unix|win32|macx {
 target.path=$$PWD/../../../bin
 INSTALLS += target
}

################################################
# Dependencies
################################################
include(../xmd/xmd.pri)

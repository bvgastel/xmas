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
include(../syntaxcheckerplugin/syntaxcheckerplugin.pri)

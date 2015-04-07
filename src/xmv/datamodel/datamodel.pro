#-------------------------------------------------
#
# Project created by QtCreator 2015-04-06T08:05:13
#
#-------------------------------------------------

QT       -= gui

TARGET = datamodel
TEMPLATE = lib

WARNINGS += -Wall
CONFIG += C++11
CONFIG += create_prl
CONFIG += link_prl

win32: CONFIG += static
unix: CONFIG += static dll

DEFINES += DATAMODEL_LIBRARY

SOURCES += \
    export.cpp \
    flatten.cpp \
    jsonprinter.cpp \
    messagespec.cpp \
    parse.cpp \
    parsed-xmas-function.cpp \
    parseflatjsonfile.cpp \
    parse-parsed-xmas-expression-interface.cpp \
    parse-specset.cpp \
    serialize_network.cpp \
    state.cpp \
    symbolic.cpp \
    symbolic-enum-field.cpp \
    symbolic-function.cpp \
    symbolic-interval-field.cpp \
    symbolic-packet-set.cpp \
#    symbolic-range-field.cpp \
    symbolic-visitor.cpp \
    xmas.cpp \
    xmasproject.cpp

HEADERS += \
    canvascomponentextension.h \
    canvas-network-extension.h \
    composite-network-extension.h \
    export.h \
    flatten.h \
    flatten-gates.h \
    jsonprinter.h \
    messagespec.h \
    parse.h \
    parsed-xmas-function.h \
    parseflatjsonfile.h \
    parse-packet-expression-parse-result.h \
    parse-packet-function-parse-result.h \
    parse-parsed-xmas-expression-interface.h \
    parse-source-expression-parse-result.h \
    parse-specset.h \
    serialize_network.h \
    state.h \
    symbolic.h \
    symbolic-enum.h \
    symbolic-enum-field.h \
    symbolic-function.h \
    symbolic-interval.h \
    symbolic-interval-field.h \
    symbolic-packet-set.h \
#    symbolic-range-field.h \
    symbolic-visitor.h \
    xmas.h \
    xmasproject.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../bitpowder/release/ -lbitpowder
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../bitpowder/debug/ -lbitpowder
else:unix: LIBS += -L$$OUT_PWD/../../bitpowder/ -lbitpowder

INCLUDEPATH += $$PWD/../../bitpowder
DEPENDPATH += $$PWD/../../bitpowder

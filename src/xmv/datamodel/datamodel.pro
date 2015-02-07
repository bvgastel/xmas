#-------------------------------------------------
#
# Project created by QtCreator 2015-01-26T14:57:19
#
#-------------------------------------------------

QT      -= qt
QT      -= core gui

TARGET = datamodel
TEMPLATE = lib

CONFIG += C++11

DEFINES += DATAMODEL_LIBRARY

SOURCES += datamodel.cpp \
    xmas.cpp \
    symbolic.cpp \
    symbolic-visitor.cpp \
    symbolic-enum-field.cpp \
    messagespec.cpp \
    export.cpp \
    serialize_network.cpp \
    symbolic-function.cpp \
    symbolic-interval-field.cpp \
#    symbolic-range-field.cpp \
    parse.cpp \
    jsonprinter.cpp \

HEADERS += datamodel.h\
        datamodel_global.h \
    xmas.h \
    symbolic.h \
    symbolic-visitor.h \
    symbolic-enum-field.h \
    messagespec.h \
    export.h \
    serialize_network.h \
    symbolic-function.h \
    symbolic-interval-field.h \
 #   symbolic-range-field.h \
    parse.h \
    jsonprinter.h \


# doesn't appear to work: should distribute header files with debug or release
#    header_files.files = $$HEADERS
#    header_files.path = $$PWD
#    QMAKE_EXTRA_TARGETS += header_files


win32 {
    # target.path = don't know
    INSTALLS += target
}

unix {
    target.path = /usr/lib
    INSTALLS += target
}

# Remark: bitpowder is external, so use $$PWD, not $$OUT_PWD.

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../build-bitpowder-Desktop-Debug/release/ -lbitpowder
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../build-bitpowder-Desktop-Debug/debug/ -lbitpowder
else:unix: LIBS += -L$$PWD/../../build-bitpowder-Desktop-Debug/ -lbitpowder

INCLUDEPATH += $$PWD/../../bitpowder
DEPENDPATH += $$PWD/../../bitpowder

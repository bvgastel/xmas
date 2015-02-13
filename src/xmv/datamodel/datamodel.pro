#-------------------------------------------------
#
# Project created by QtCreator 2015-01-26T14:57:19
#
#-------------------------------------------------
#
# Instructions for use:
#
# 1. When building, the output goes solely to the
#    build directory. This is usually something
#    like build-libraryname-platform-compiler
#    where libraryname, platform and compiler vary.
# 2. When cleaning, only the build directory is cleaned.
#
# 3. When deploying, the library files (both dll/so and
#    .a) and the header files are copied to the lib and
#    include directory right below the git-root. This is
#    the most vulnerable piece of code.
#    REMARK: when cleaning, this does not get touched.
# 3a. The include directory has a subdir for this project's
#     header files.
#
# IMPORTANT: Be sure to have qtcreator execute a make install
#            as one step in the local deployment.
#

#QT      -= qt
#QT      -= core gui
QT -= gui

TARGET = datamodel
TEMPLATE = lib

CONFIG += C++11
CONFIG += create_prl
CONFIG += link_prl
win32: CONFIG += static
unix: CONFIG += static dll

DEFINES += DATAMODEL_LIBRARY

SOURCES += datamodel.cpp \
    xmas.cpp \
    symbolic.cpp \
    symbolic-visitor.cpp \
    symbolic-enum-field.cpp \
    messagespec.cpp \
#    export.cpp \
    symbolic-function.cpp \
    symbolic-interval-field.cpp \
#    symbolic-range-field.cpp \
    parse.cpp \
    jsonprinter.cpp \
    serialize_network.cpp

HEADERS += datamodel.h\
        datamodel_global.h \
    xmas.h \
    symbolic.h \
    symbolic-visitor.h \
    symbolic-enum-field.h \
    messagespec.h \
    export.h \
    symbolic-function.h \
    symbolic-interval-field.h \
 #   symbolic-range-field.h \
    parse.h \
    jsonprinter.h \
    serialize_network.h

unix|win32 {
    target.path = $$PWD/../../../lib/datamodel
    INSTALLS += target

    headerfiles.path=$$PWD/../../../include/datamodel
    headerfiles.files = $$PWD/*.h
    INSTALLS += headerfiles

}

# Remark: bitpowder is external, so use $$PWD, not $$OUT_PWD.
unix|win32: LIBS += -L$$PWD/../../../lib/bitpowder -lbitpowder

INCLUDEPATH += $$PWD/../../../include/bitpowder
DEPENDPATH += $$PWD/../../../include/bitpowder

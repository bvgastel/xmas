#-------------------------------------------------
#
# Project created by QtCreator 2015-01-27T15:32:47
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
TEMPLATE = lib

WARNINGS += -Wall

QT      -= qt
QT       -= core gui

CONFIG += C++11
CONFIG += create_prl
CONFIG += link_prt
CONFIG += static dll
#CONFIG += build_all

TARGET = bitpowder
CONFIG(debug, debug|release) {
    macx: TARGET = $$join(TARGET,,,_debug)
    win32: TARGET = $$join(TARGET,,,d)
}

DEFINES += BITPOWDER_LIBRARY

SOURCES += \
    dispatch.cpp \
    exception.cpp \
    iterators.cpp \
    memory.cpp \
    memorypool.cpp \
    memorypool.debug.cpp \
    parser.cpp \
    parser_json.cpp \
    queue.cpp \
    shared_object.cpp \
    simplestring.cpp \
    stack.cpp \
    stringparse.cpp \
    thread.cpp \
    type_hash.cpp \


HEADERS += \
    bitpowder_global.h \
    atomic.h \
    common.h \
    deque.h \
    dispatch.h \
    exception.h \
    extension.h \
    iterators.h \
    lock.h \
    memory.h \
    memorypool.debug.h \
    memorypool.h \
    parser.h \
    parser_json.h \
    queue.h \
    semaphore.h \
    shared_object.h \
    simplestring.h \
    spinlock.h \
    stack.h \
    stringparse.h \
    thread.h \
    type_hash.h \
    zip.h \

################################################
# INSTALL instructions
################################################
unix|win32|macx {
    target.path = $$PWD/../../lib/bitpowder
    INSTALLS += target

    headerfiles.path=$$PWD/../../include/bitpowder
    headerfiles.files = $$PWD/*.h
    INSTALLS += headerfiles
}

################################################
# Internal dependencies
################################################


################################################
# External dependencies
################################################




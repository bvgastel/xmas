#-------------------------------------------------
#
# Project created by QtCreator 2015-04-06T07:35:02
#
#-------------------------------------------------

QT       -= core gui qt

WARNINGS += -Wall

CONFIG += C++11
CONFIG += create_prl
CONFIG += link_prl
win32|linux:CONFIG += static dll
macx: CONFIG += staticlib

TARGET = bitpowder
TEMPLATE = lib

DEFINES += BITPOWDER_LIBRARY

SOURCES += \
    dispatch.cpp \
    exception.cpp \
#    iterators.cpp \
    memory.cpp \
    memorypool.cpp \
    parser.cpp \
    parser_json.cpp \
#    queue.cpp \
#    shared_object.cpp \
    simplestring.cpp \
#    stack.cpp \
#    stringparse.cpp \
    thread.cpp \
#    type_hash.cpp

HEADERS += \
    atomic.h \
    common.h \
    deque.h \
    dispatch.h \
    exception.h \
    extension.h \
    iterators.h \
    lock.h \
    memory.h \
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
    zip.h


################################################
# INSTALL instructions
################################################
unix|win32|macx {
    target.path = $$PWD/../../lib
    INSTALLS += target
}

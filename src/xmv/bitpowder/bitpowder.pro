#-------------------------------------------------
#
# Project created by QtCreator 2015-01-27T15:32:47
#
#-------------------------------------------------

QT       -= core gui

TARGET = bitpowder
TEMPLATE = lib

CONFIG += C++11

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
    type_hash.cpp

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
    zip.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

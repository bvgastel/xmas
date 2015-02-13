#-------------------------------------------------
#
# Project created by QtCreator 2015-02-05T15:47:26
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

#QT       -= qt
#QT       -= core gui
QT -= gui

TARGET = vt
TEMPLATE = lib

CONFIG += C++11
CONFIG += create_prl
CONFIG += link_prl
CONFIG += static dll

DEFINES += VT_LIBRARY

SOURCES += vt.cpp \
    cycle.cpp \
#   constraints.cpp \       # does not compile. Used nowhere.
    deadlock.cpp

HEADERS += vt.h\
        vt_global.h \
    cycle.h \
#   constraints.h \         # does not compile. Only used in constraints.cpp
    deadlock.h

win32 {
    target.path = $$PWD/../../../lib/vt
    INSTALLS += target

    headerfiles.path=$$PWD/../../../include/vt
    headerfiles.files = $$PWD/*.h
    INSTALLS += headerfiles

}

unix {
    target.path = $$PWD/../../../lib/vt
    INSTALLS += target

    headerfiles.path=$$PWD/../../../include/vt
    headerfiles.files = $$PWD/*.h
    INSTALLS += headerfiles

}
############################################

# Remark: bitpowder is external, so use $$PWD, not $$OUT_PWD.

unix|win32: LIBS += -L$$PWD/../../../lib/bitpowder -lbitpowder

INCLUDEPATH += $$PWD/../../../include/bitpowder
DEPENDPATH += $$PWD/../../../include/bitpowder

DISTFILES += \
    build-convert.sh


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../datamodel/release -ldatamodel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../datamodel/debug -ldatamodel
else:unix: LIBS += -L$$OUT_PWD/../datamodel/ -ldatamodel

INCLUDEPATH += $$PWD/../datamodel
DEPENDPATH += $$PWD/../datamodel

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

TEMPLATE = lib

WARNINGS += -Wall

QT -= gui

CONFIG += C++11
CONFIG += create_prl
CONFIG += link_prl
win32: CONFIG += static
unix: CONFIG += static
#CONFIG += build_all

TARGET = vt
CONFIG(debug, debug|release) {
    macx: TARGET = $$join(TARGET,,,_debug)
    win32: TARGET = $$join(TARGET,,,d)
}

DEFINES += VT_LIBRARY

SOURCES += vt.cpp \
    cycle.cpp \
    deadlock.cpp

HEADERS += vt.h\
        vt_global.h \
    cycle.h \
    deadlock.h

DISTFILES += \
    build-convert.sh

################################################
# INSTALL instructions
################################################
unix|win32|macx {
    target.path = $$PWD/../../../lib/vt
    INSTALLS += target

    headerfiles.path=$$PWD/../../../include/vt
    headerfiles.files = $$HEADERS
    INSTALLS += headerfiles
}

################################################
# Internal dependencies
################################################
macx:CONFIG(debug, debug|release): LIBS += \
    -L$$OUT_PWD/../datamodel/ -ldatamodel_debug
else:macx:CONFIG(release, debug|release): LIBS += \
    -L$$OUT_PWD/../datamodel/ -ldatamodel
else:win32:CONFIG(debug, debug|release): LIBS += \
    -L$$OUT_PWD/../datamodel/debug/ -ldatamodeld
else:win32:CONFIG(release, debug|release): LIBS += \
    -L$$OUT_PWD/../datamodel/release/ -ldatamodel
else:unix: LIBS += \
    -L$$OUT_PWD/../datamodel/ -ldatamodel

INCLUDEPATH += $$PWD/../datamodel
DEPENDPATH += $$PWD/../datamodel

################################################
# External dependencies
################################################
macx:CONFIG(debug, debug|release): LIBS += \
    -L$$PWD/../../../lib/bitpowder/ -lbitpowder_debug

else:win32:CONFIG(debug, debug|release): LIBS += \
    -L$$PWD/../../../lib/bitpowder/ -lbitpowderd

else:unix|CONFIG(release, debug|release): LIBS += \
    -L$$PWD/../../../lib/bitpowder/ -lbitpowder

INCLUDEPATH += $$PWD/../../../include/bitpowder
DEPENDPATH += $$PWD/../../../include/bitpowder

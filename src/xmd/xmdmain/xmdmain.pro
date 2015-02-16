# xmdmain.pro
# Contains the main program for xmd

# Be sure to add deploy step with make (parameter install)

CONFIG += C++11

QT += widgets svg quick qml quickwidgets

TEMPLATE = app
CONFIG += link_prl

HEADERS       = \

SOURCES += \
    main.cpp

unix|win32 {
 target.path=$$PWD/../../../bin
 INSTALLS += target
}

unix|win32: LIBS += -L$$PWD/../../../lib/datamodel -ldatamodel
unix|win32: LIBS += -L$$PWD/../../../lib/bitpowder -lbitpowder

INCLUDEPATH += $$PWD/../../../include/datamodel
DEPENDPATH += $$PWD/../../../include/datamodel

INCLUDEPATH += $$PWD/../../../include/bitpowder
DEPENDPATH += $$PWD/../../../include/bitpowder

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../xmd/release/ -lxmd
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../xmd/debug/ -lxmd
else:unix: LIBS += -L$$OUT_PWD/../xmd/ -lxmd

INCLUDEPATH += $$PWD/../xmd
DEPENDPATH += $$PWD/../xmd

RESOURCES +=

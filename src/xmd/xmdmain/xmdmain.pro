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

INCLUDEPATH += $$PWD/../xmd $$PWD/../xmd/xmapper $$PWD/../xmd/vplugin
DEPENDPATH += $$PWD/../xmd $$PWD/../xmd/xmapper $$PWD/../xmd/vplugin

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../plugins/release/ -lplugins
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../plugins/debug/ -lplugins
else:unix: LIBS += -L$$OUT_PWD/../plugins/ -lplugins

INCLUDEPATH += $$PWD/../plugins
DEPENDPATH += $$PWD/../plugins

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../xmd/release/libxmd.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../xmd/debug/libxmd.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../xmd/release/xmd.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../xmd/debug/xmd.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../xmd/libxmd.a

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../plugins/release/libplugins.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../plugins/debug/libplugins.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../plugins/release/plugins.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../plugins/debug/plugins.lib
//else:unix: PRE_TARGETDEPS += $$OUT_PWD/../xmd/libplugins.a



RESOURCES +=

DISTFILES += \
    readme.md

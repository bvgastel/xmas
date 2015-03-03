TEMPLATE = app

WARNINGS += -Wall

CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += C++11
CONFIG += link_prl

include(deployment.pri)
qtcAddDeployment()

SOURCES += main.cpp

################################################
# INSTALL instructions
################################################
unix|win32|macx {
 target.path=$$PWD/../../../bin
 INSTALLS += target
}

################################################
# Internal dependencies
################################################
macx:CONFIG(debug, debug|release): LIBS += \
    -L$$OUT_PWD/../datamodel/ -ldatamodel_debug \
    -L$$OUT_PWD/../vt/ -lvt_debug
else:macx:CONFIG(release, debug|release): LIBS += \
    -L$$OUT_PWD/../datamodel/ -ldatamodel \
    -L$$OUT_PWD/../vt/ -lvt
else:win32:CONFIG(debug, debug|release): LIBS += \
    -L$$OUT_PWD/../datamodel/debug/ -ldatamodeld \
    -L$$OUT_PWD/../vt/debug/ -lvtd
else:win32:CONFIG(release, debug|release): LIBS += \
    -L$$OUT_PWD/../datamodel/release/ -ldatamodel \
    -L$$OUT_PWD/../vt/release/ -lvt
else:unix: LIBS += \
    -L$$OUT_PWD/../datamodel/ -ldatamodel \
    -L$$OUT_PWD/../vt/ -lvt

INCLUDEPATH += $$PWD/../datamodel
DEPENDPATH += $$PWD/../datamodel

INCLUDEPATH += $$PWD/../vt
DEPENDPATH += $$PWD/../vt

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

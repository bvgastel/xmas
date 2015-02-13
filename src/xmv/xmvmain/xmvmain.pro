TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

include(deployment.pri)
qtcAddDeployment()

CONFIG += C++11
CONFIG += link_prl

SOURCES += main.cpp

unix|win32 {
 target.path=$$PWD/../../../bin
 INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../datamodel/release/ -ldatamodel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../datamodel/debug/ -ldatamodel
else:unix: LIBS += -L$$OUT_PWD/../datamodel/ -ldatamodel

INCLUDEPATH += $$PWD/../datamodel
DEPENDPATH += $$PWD/../datamodel

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../vt/release/ -lvt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../vt/debug/ -lvt
else:unix: LIBS += -L$$OUT_PWD/../vt/ -lvt

INCLUDEPATH += $$PWD/../vt
DEPENDPATH += $$PWD/../vt

# Remark: bitpowder is external, so use $$PWD, not $$OUT_PWD.

unix|win32: LIBS += -L$$PWD/../../../lib/bitpowder -lbitpowder

INCLUDEPATH += $$PWD/../../../include/bitpowder
DEPENDPATH += $$PWD/../../../include/bitpowder

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

HEADERS +=


SOURCES += \
    main.cpp \
    testcyclechecker.cpp \
    testdatamodel.cpp \


CONFIG += C++11

include(deployment.pri)
qtcAddDeployment()

# Google test library
unix|win32: LIBS += -lgtest_main -lgtest

# xmv/datamodel debug library

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-xmv-Desktop-Debug/datamodel/release/ -ldatamodel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-xmv-Desktop-Debug/datamodel/debug/ -ldatamodel
else:unix: LIBS += -L$$PWD/../build-xmv-Desktop-Debug/datamodel/ -ldatamodel

INCLUDEPATH += $$PWD/../xmv/datamodel
DEPENDPATH += $$PWD/../xmv/datamodel

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-xmv-Desktop-Debug/vt/release/ -lvt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-xmv-Desktop-Debug/vt/debug/ -lvt
else:unix: LIBS += -L$$PWD/../build-xmv-Desktop-Debug/vt/ -lvt

INCLUDEPATH += $$PWD/../xmv/vt
DEPENDPATH += $$PWD/../xmv/vt

# Remark: bitpowder is external, so use $$PWD, not $$OUT_PWD.

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-bitpowder-Desktop-Debug/release/ -lbitpowder
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-bitpowder-Desktop-Debug/debug/ -lbitpowder
else:unix: LIBS += -L$$PWD/../build-bitpowder-Desktop-Debug/ -lbitpowder

INCLUDEPATH += $$PWD/../bitpowder
DEPENDPATH += $$PWD/../build-bitpowder-Desktop-Debug

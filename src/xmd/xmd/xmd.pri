include(../../bitpowder/bitpowder.pri)
include(../../xmv/vt/vt.pri)
include(../../interfaces/interfaces.pri)

QT += qml quick

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

LIBS += -lxmd

win32-msvc {
        CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../xmd/release/
        CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../xmd/debug/

        CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../xmd/release/xmd.lib
        CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../xmd/debug/xmd.lib
}

win32-g++ {
        CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../xmd/release/
        CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../xmd/debug/

        CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../xmd/release/libxmd.a
        CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../xmd/debug/libxmd.a
}


unix {
        LIBS += -L$$OUT_PWD/../xmd/
        macx: PRE_TARGETDEPS += $$OUT_PWD/../xmd/libxmd.a

        QMAKE_LFLAGS += -fprofile-arcs
}

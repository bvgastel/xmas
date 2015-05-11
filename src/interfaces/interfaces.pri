include(../xmv/vt/vt.pri)

QT += qml quick

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

LIBS += -linterfaces

win32-msvc {
        CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../interfaces/release/
        CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../interfaces/debug/

        CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../interfaces/release/interfaces.lib
        CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../interfaces/debug/interfaces.lib
}

win32-g++ {
        CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/$$BASE/interfaces/release/
        CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/$$BASE/interfaces/debug/

        CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/$$BASE/interfaces/release/libinterfaces.a
        CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/$$BASE/interfaces/debug/libinterfaces.a
}

unix {
        LIBS += -L$$OUT_PWD/../../interfaces/
        macx: PRE_TARGETDEPS += $$OUT_PWD/../../interfaces/libinterfaces.a

        QMAKE_LFLAGS += -fprofile-arcs
}

include(../xmv/datamodel/datamodel.pri)

QT += qml quick

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

LIBS += -linterfaces

win32 {
        CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../interfaces/release/
        CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../interfaces/debug/

        CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../interfaces/release/interfaces.lib
        CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../interfaces/debug/interfaces.lib
}

unix {
        LIBS += -L$$OUT_PWD/../../interfaces/
        PRE_TARGETDEPS += $$OUT_PWD/../../interfaces/libinterfaces.a

        QMAKE_LFLAGS += -fprofile-arcs
}

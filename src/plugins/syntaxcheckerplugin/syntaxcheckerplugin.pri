include(../../interfaces/interfaces.pri)

QT += qml quick

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

LIBS += -lsyntaxcheckermodel

win32 {
        CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/$$BASE/plugins/syntaxcheckermodel/release/
        CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/$$BASE/plugins/syntaxcheckermodel/debug/

        CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/$$BASE/plugins/syntaxcheckermodel/release/syntaxcheckermodel.lib
        CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/$$BASE/plugins/syntaxcheckermodel/debug/syntaxcheckermodel.lib
}

unix {
        LIBS += -L$$OUT_PWD/$$BASE/plugins/syntaxcheckermodel/
        macx: PRE_TARGETDEPS += $$OUT_PWD/$$BASE/plugins/syntaxcheckermodel/libsyntaxcheckermodel.a

	QMAKE_LFLAGS += -fprofile-arcs
}

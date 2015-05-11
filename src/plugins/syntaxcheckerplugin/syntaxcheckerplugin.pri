include(../../interfaces/interfaces.pri)

QT += qml quick

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

LIBS += -lsyntaxcheckerplugin

win32-msvc {
        CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/$$BASE/plugins/syntaxcheckerplugin/release/
        CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/$$BASE/plugins/syntaxcheckerplugin/debug/

        CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/$$BASE/plugins/syntaxcheckerplugin/release/syntaxcheckerplugin.lib
        CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/$$BASE/plugins/syntaxcheckerplugin/debug/syntaxcheckerplugin.lib
}

win32-g++ {
        CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/$$BASE/plugins/syntaxcheckerplugin/release/
        CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/$$BASE/plugins/syntaxcheckerplugin/debug/

        CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/$$BASE/plugins/syntaxcheckerplugin/release/libsyntaxcheckerplugin.a
        CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/$$BASE/plugins/syntaxcheckerplugin/debug/libsyntaxcheckerplugin.a
}

unix {
        LIBS += -L$$OUT_PWD/$$BASE/plugins/syntaxcheckerplugin/
        macx: PRE_TARGETDEPS += $$OUT_PWD/$$BASE/plugins/syntaxcheckerplugin/libsyntaxcheckerplugin.a

	QMAKE_LFLAGS += -fprofile-arcs
}

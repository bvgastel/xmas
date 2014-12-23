#ifndef CONTROL_H
#define CONTROL_H

#include <string>
#include <iostream>
#include <memory>

#include <QCoreApplication>
#include <QApplication>
#include <QCommandLineParser>
#include <QVector>

#include "setup.h"

struct Options {
    QStringList args;
    bool commandline;
    bool runAll;
    QString outputDir;
};

class Control
{
public:
    Control(int argc, char *argv[]);
    ~Control();

    int exec();

private:
    std::shared_ptr<QApplication> m_app;
    std::shared_ptr<Options> m_opt;

    std::shared_ptr<Options> getOptions();
};

#endif // CONTROL_H

#include <iostream>
#include <string>

#include <QCoreApplication>
#include <QApplication>
#include <QCommandLineParser>
#include <QVector>

#include "setup.h"
#include "control.h"

int main(int argc, char *argv[])
{
    Control control(argc, argv);

    int result = control.exec();
    std::cout << "Application result returned was " << result << std::endl;

    return result;
}

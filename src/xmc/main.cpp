#include <iostream>
#include <string>
#include <memory>

#include <QCoreApplication>
#include <QApplication>
#include <QCommandLineParser>
#include <QVector>

#include "setup.h"
#include "control.h"

int main(int argc, char *argv[])
{
    std::shared_ptr<Control> control = std::make_shared<Control>(argc, argv);

    int result = control->exec();

    std::cout << "Application result returned was " << result;

    return result;
}

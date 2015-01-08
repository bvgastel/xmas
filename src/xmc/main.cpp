#include <iostream>
#include <string>

#include <QCoreApplication>
#include <QApplication>
#include <QCommandLineParser>
#include <QVector>

#include "setup.h"
#include "control.h"

/**
 * A test application to drive the controller. In future this
 * may or may not be the same driver.
 *
 *
 * @brief main
 * @param argc The number of arguments on the commandline
 * @param argv The arguments on the command line. Arg[0] == the program name.
 * @return zero if ok, else non-zero.
 */

int main(int argc, char *argv[])
{
    Control control(argc, argv);

    int result = control.exec();
    std::cout << "Application result returned was " << result << std::endl;

    return result;
}

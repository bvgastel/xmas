/*********************************************************************
  *
  * Copyright (C) Guus Bonnema, 2015
  *
  * This file is part of the xmas-design tool.
  *
  * The xmas-design tool is free software: you can redistribute it
  * and/or modify it under the terms of the GNU General Public License
  * as published by the Free Software Foundation, either version 3 of
  * the License, or (at your option) any later version.
  *
  * The xmas-design tool is distributed in the hope that it will be
  * useful,  but WITHOUT ANY WARRANTY; without even the implied warranty
  * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with the xmas-design tool.  If not, see
  * <http://www.gnu.org/licenses/>.
  *
  **********************************************************************/

#include <iostream>

#include <QCoreApplication>
#include <QBuffer>
#include <QSharedMemory>
#include <QDataStream>

#include "xmasproject.h"
#include "workerinterface.h"
#include "syntaxcheckworker.h"

// Example how to get json using shared memory. Not tested :-D
std::pair<bool, QString> json() {

    QSharedMemory sharedMemory;

    if (!sharedMemory.attach()) {
        return std::make_pair<bool, QString>(false, QString());
    }

    QBuffer buffer;
    QDataStream in(&buffer);
    QString json;

    sharedMemory.lock();
    buffer.setData((char*)sharedMemory.constData(), sharedMemory.size());
    buffer.open(QBuffer::ReadOnly);
    in >> json;
    sharedMemory.unlock();

    sharedMemory.detach();
    return std::make_pair<bool, QString>(false, QString());
}

int main(int argc, char *argv[])
{
    std::cout << "argc = " << argc << ", argv = '" << *argv << "'" << std::endl;

    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("syntax checker main");
    QCoreApplication::setApplicationVersion("1.0");

    // Unfortunately link-edit for the syntaxcheckworker, only is successful
    // from Linux, not from Windows. Could be a build option, but I couldn't
    // find the culprit.
    // Probably the fact the it is in a plugin library. It might have to be in a
    // different (non-plugin) library
//    WorkerInterface *plugin = new SyntaxCheckWorker();
    QString qjson;
    std::string jsonStr;
    bool success;
    std::tie(success, qjson)= json();
    jsonStr = qjson.toStdString();
    if (success) {
        std::cout << "Received json string: '" << jsonStr << "'" << std::endl;
        std::string networkName = "syntax checker plugin";
        std::string basePath = "";
        std::shared_ptr<XMASProject> project = std::make_shared<XMASProject>(jsonStr, networkName, basePath);
//        plugin->doWork(project);
        // Add the code for the plugin here
        //
        // plugin-code
        // plugin-code
        // etc etc
        //
        return 0;
    }
    return -1;

    //return app.exec();        // without app.exec() no event thread !!

//    std::cout << "Thanks for deploying the most recently developed plugins." << std::endl;
//    std::cout << "******* Program finishes ********************************" << std::endl;
//    exit(0);

}

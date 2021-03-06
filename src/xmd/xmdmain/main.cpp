/************************************************************************
 *
 * Copyright Stefan Versluys, 2014
 *
 * This file is part of the xmas-design tool.
 *
 *  The xmas-design tool is free software: you can redistribute it
 *  and/or modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation, either version 3 of
 *  the License, or (at your option) any later version.
 *
 *  The xmas-design tool is distributed in the hope that it will be
 *  useful,  but WITHOUT ANY WARRANTY; without even the implied warranty
 *  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with the xmas-design tool.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 *
 * Parts of this code were copied from Qt examples with following copyright
 * and license notices:
 *
 * Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies)
 *
 * under BSD license.
 *
 *
 **************************************************************************/

#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QQmlApplicationEngine>

#include <QQmlContext>
#include <QtQml>
#include "memorypool.h"
#include "plugincontrol.h"
#include "datacontrol.h"
#include "model/util.h"
#include "model/network.h"


/*
 * Access globals in C++ through statements like
 *
 *      "extern DataControl *dataControl;"
 *
 */
std::unique_ptr<DataControl> dataControl = nullptr;
std::unique_ptr<PluginControl> pluginControl = nullptr;
std::unique_ptr<Util> util = nullptr;

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(quick);
    Q_INIT_RESOURCE(javascripts);
    Q_INIT_RESOURCE(images);

    QApplication app(argc, argv);
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);
    QCoreApplication::setApplicationName("XMAS Model Designer (2015)");
    QCoreApplication::setOrganizationName("Open University NL");
    QCoreApplication::setOrganizationDomain("ou.nl");

    /*************************************************/
    /* OOAK class registration for Qml access        */

    QQmlApplicationEngine engine;
    QQmlContext* ctx = engine.rootContext();

    ::dataControl.reset(new DataControl);
    qmlRegisterType<DataControl>("XMAS", 1, 0, "Data"); // Before engine.load
    ctx->setContextProperty("datacontrol", dataControl.get());

    ::pluginControl.reset(new PluginControl);
    qmlRegisterType<PluginControl>("XMAS", 1, 0, "Plugin"); // Before engine.load
    ctx->setContextProperty("plugincontrol", pluginControl.get());

    ::util.reset(new Util());
    qmlRegisterType<Util>("XMAS", 1, 0, "Util"); // Before engine.load
    ctx->setContextProperty("util", util.get());


    /* End of OOAK class registration for Qml access */
    /*************************************************/

    dataControl->registerTypes(); // Before engine.load

    engine.load(QUrl(QStringLiteral("qrc:/ui/mainWindow.qml")));
    return app.exec();

}

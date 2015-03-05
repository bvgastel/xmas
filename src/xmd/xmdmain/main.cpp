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
#include "plugincontrol.h"
#include "datacontrol.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(xmd);

    QApplication app(argc, argv);
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);

    //Important! First register then engine.load
    // otherwise c++ types will not be seen by qml
    qmlRegisterType<DataControl>("XMAS", 1, 0, "Data");

    //andere voorbeelden voor extentie classes
//    qmlRegisterType<DataControl>("XMAS", 1, 0, "Component");
//    qmlRegisterType<DataControl>("XMAS", 1, 0, "Channel");
//    qmlRegisterType<DataControl>("XMAS", 1, 0, "Port");
    qmlRegisterType<PluginControl>("XMAS", 1, 0, "Plugin");


    QQmlApplicationEngine engine;
    PluginControl pluginControl;
    DataControl dataControl;

    QQmlContext* ctx = engine.rootContext();
    ctx->setContextProperty("plugincontrol", &pluginControl);
    ctx->setContextProperty("datacontrol", &dataControl);

    engine.load(QUrl(QStringLiteral("qrc:///mainWindow.qml")));


    return app.exec();

}

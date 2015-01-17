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
#include <QQuickView>

#include "canvas/component.h"
#include "canvas/connector.h"
#include "canvas/connection.h"
#include "canvas/network.h"

//#include "mainwindow.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(xmd);

    QApplication app(argc, argv);
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);
    QCommandLineParser parser;
    parser.setApplicationDescription("XMAS Model Designer");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", "The file to open.");
    parser.process(app);

    qmlRegisterType<Component>("XMAS", 1, 0, "XComponent");
    qmlRegisterType<Connector>("XMAS", 1, 0, "XConnector");
    qmlRegisterType<Connection>("XMAS", 1, 0, "XConnection");

    QQuickView *view = new QQuickView;
    view->setResizeMode(QQuickView::SizeRootObjectToView);
    view->setSource(QUrl("qrc:/dynamicscene.qml"));
    view->show();



//    Network *n = new Network;
//    n->setName("model1");

//    Component *c1 = new Component;

//    c1->setName("comp1");
//    c1->setPosition(QPoint(100,200));
//    c1->setOrientation(Component::Orientation::Up);


//    n->append_component(n->components()&,c1);



//    QFile file("file.dat");
//    file.open(QIODevice::WriteOnly);

//    QDataStream out(&file);   // we will serialize the data into the file
//    out <<n;


//    //file.open(QIODevice::ReadOnly);
//    //out >> n;

//        file.close();

//qDebug() << "Write network name: " << n->name() << ", component name: " << c1->name;


//    MainWindow mainWin;
//    foreach (const QString &fileName, parser.positionalArguments())
//        mainWin.openFile(fileName);
//    mainWin.show();
    return app.exec();
}

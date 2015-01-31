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
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QVariant>
#include <QDebug>
#include <QQmlApplicationEngine>

class Controller : public QObject
{
    Q_OBJECT

signals: //to view
    void componentCreate(QVariant object);
    bool createConnection(QVariant object);
    bool clearNetwork();

public slots:  //from view
    bool componentCreated(QVariant object);
    bool componentDestroyed(QVariant object);
    bool componentChanged(QVariant object);
    bool connectionCreated(QVariant object);
    bool connectionDestroyed(QVariant object);
    bool connectionChanged(QVariant object);

    //TODO : test can be removed later (stefan)
    bool testClicked(); //connected to the paste button on the toolbar
public:
    explicit Controller(QObject* parent = 0);

    ~Controller();

private:
    bool scratch();

    QQmlApplicationEngine engine;
    QObject *m_window;

};

#endif // CONTROLLER_H










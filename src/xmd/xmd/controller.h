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

#include <unordered_map>
#include <typeindex>

#include <QVariant>
#include <QVariantMap>
#include <QColor>
#include <QDebug>
#include <QMap>
#include <QUrl>

#include "xmas.h"

class Controller : public QObject
{
    Q_OBJECT
    Q_ENUMS(Orientation)
    Q_ENUMS(PortType)
    Q_ENUMS(CompType)

signals: //to view
    //void createComponent(QString type, QObject *object);
    void createComponent(const QVariantMap &object);
    bool createChannel(QVariant object);
    bool clearNetwork();
    void log(QString message,QColor color);

public slots:  //from view
    bool componentCreated(QVariant object);
    bool componentDestroyed(QVariant object);
    bool componentChanged(QVariant object);
    bool channelCreated(QVariant object);
    bool channelDestroyed(QVariant object);
    bool channelChanged(QVariant object);
    bool fileOpen(QUrl fileUrl);

public:
    enum Orientation {
        North = 0,
        East = 90,
        South = 180,
        West = 270,
        NorthWest = 45,
        SouthWest = 225,
        NorthEast = 315,
        SouthEast = 135
    };
    enum PortType {Target , Initiator};
    enum CompType {Source, Sink, Function, Queue, Join, Merge, Switch, Fork};

    explicit Controller(QObject* parent = 0);
    ~Controller();

private:
    bool scratch();

    QVariant createPropObject(XMASComponent *comp);

    void controllerLog(const std::string message,QColor color);
    void controllerLog(const std::string message);

    void controllerLog(const QString message, QColor color);
    void controllerLog(const QString message);

    void controllerLog(const bitpowder::lib::String message, QColor color);
    void controllerLog(const bitpowder::lib::String message);

    void log(const QString message);
    void log(const std::string message, QColor color = Qt::black);
    void log(const bitpowder::lib::String message, QColor color = Qt::black);

    void emitComponent(XMASComponent *comp);


    std::map<std::type_index, QString> m_type_map = {
        {std::type_index(typeid(XMASSource)),   "source" },
        {std::type_index(typeid(XMASSink)),     "sink" },
        {std::type_index(typeid(XMASFunction)), "function" },
        {std::type_index(typeid(XMASQueue)),    "queue"},
        {std::type_index(typeid(XMASJoin)),     "join"},
        {std::type_index(typeid(XMASMerge)),    "merge"},
        {std::type_index(typeid(XMASFork)),     "fork"},
        {std::type_index(typeid(XMASSwitch)),   "switch"},
    };


    //TODO : enumeration in javascript.
//    std::map<std::type_index, CompType> m_type_map = {
//        {std::type_index(typeid(XMASSource)),   CompType::Source },
//        {std::type_index(typeid(XMASSink)),     CompType::Sink },
//        {std::type_index(typeid(XMASFunction)), CompType::Function },
//        {std::type_index(typeid(XMASQueue)),    CompType::Queue},
//        {std::type_index(typeid(XMASJoin)),     CompType::Join},
//        {std::type_index(typeid(XMASMerge)),    CompType::Merge},
//        {std::type_index(typeid(XMASFork)),     CompType::Fork},
//        {std::type_index(typeid(XMASSwitch)),   CompType::Switch},
//    };


};

#endif // CONTROLLER_H

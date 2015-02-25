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

#include <map>
#include <set>
#include <typeindex>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>

#include <QVariant>
#include <QVariantMap>
#include <QColor>
#include <QDebug>
#include <QMap>
#include <QUrl>

#include "xmas.h"
#include "componentwalker.h"
#include "vtplugininterface.h"
#include "logger.h"


class Controller : public QObject
{
    Q_OBJECT
    Q_ENUMS(Orientation)
    Q_ENUMS(PortType)
    Q_ENUMS(CompType)

signals: //to view
    void createComponent(const QVariantMap &object);
    bool createChannel(const QVariantMap &object);
    bool clearNetwork();
    void writeLog(QString message,QColor color=Qt::black);
    /**
     * @brief createNetwork a signal with the network components and connections in a list
     *
     * The input for the processing slot is a QVariantList (alias for QList<QVariant>) containing
     * two other QVariantList occurences:
     *
     * 1. A component list containing all components. These need processing before the channels.
     *      The key for component list is "complist"
     * 2. A channel list, connecting the previously listed components.
     *      The key for channel list is "channellist"
     *
     * Remark: this signal means to include an implicit, pre-executed clearNetwork();
     *
     * @param object The object is a list containing 2 lists: component list and channel list.
     * @return true if successful
     */
    bool createNetwork(const QVariantMap &object);

public slots:  //from view
    bool componentCreated(const QVariant &object);
    bool componentDestroyed(const QVariant &object);
    bool componentChanged(const QVariant &object);
    bool channelCreated(const QVariant &object);
    bool channelDestroyed(const QVariant &object);
    bool channelChanged(const QVariant &object);
    bool fileOpen(QUrl fileUrl);
    size_t loadPlugins();

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

    QVariantMap pluginParams(QString name);
    bool pluginParam(QString name, QString key, QString value);

//    void controllerLog(const std::string message,QColor color=QColor("black"));
//    void controllerLog(const QString message, QColor color=QColor("black"));
//    void controllerLog(const bitpowder::lib::String message, QColor color=QColor("black"));

private:
    QVariant createPropObject(XMASComponent *comp);

    void emitComponent(XMASComponent *comp);
    void emitDuplicate(XMASComponent *comp);
    bool emitNetwork();

    void convertToQml(QVariantMap &map, XMASComponent *comp);
    void connectInQml(QVariantList &list, XMASComponent *comp);

private:

    std::shared_ptr<ComponentWalker> m_componentWalker;
    /**
     * @brief allComponents The internal structure containing the network
     *
     */
    std::map<bitpowder::lib::String, XMASComponent *> m_componentMap;

    //TODO : enumeration in javascript.
    std::map<std::type_index, QString> m_type_map = {
        {std::type_index(typeid(XMASSource)), "source" },
        {std::type_index(typeid(XMASSink)), "sink" },
        {std::type_index(typeid(XMASFunction)), "function" },
        {std::type_index(typeid(XMASQueue)), "queue"},
        {std::type_index(typeid(XMASJoin)), "join"},
        {std::type_index(typeid(XMASMerge)), "merge"},
        {std::type_index(typeid(XMASFork)), "fork"},
        {std::type_index(typeid(XMASSwitch)), "switch"},
    };

    QMap<QString, VtPluginInterface *> m_vtMap;
    Logger m_logger;
};

#endif // CONTROLLER_H

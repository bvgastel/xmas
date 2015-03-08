/*********************************************************************
  *
  * Copyright (C) <name>, <year>
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

#ifndef DATACONTROL_H
#define DATACONTROL_H

#include <typeindex>
#include <map>

#include <QObject>
#include <QColor>
#include <QUrl>

#include "simplestring.h"
#include "xmas.h"
#include "logger.h"
#include "parse.h"
#include "memorypool.h"
#include "exception.h"
#include "common.h" // for destroy template

typedef std::map<bitpowder::lib::String, XMASComponent *> XCompMap;
typedef bitpowder::lib::MemoryPool XMP;

const QString xsource = "source";
const QString xsink = "sink";
const QString xfunction = "function";
const QString xqueue = "queue";
const QString xjoin = "join";
const QString xmerge = "merge";
const QString xfork = "fork";
const QString xswitch = "switch";
const QString xin = "in";
const QString xout = "out";

/**
 * @brief The DataControl class
 *
 * The data control class has access to the complete datamodel and is able to
 * manage the data (create, modify, destroy and cleanup).
 * It also registers the data classes with Qml as required.
 *
 *
 *
 */
class DataControl : public QObject
{
    Q_OBJECT
    Q_ENUMS(Orientation)
    Q_ENUMS(PortType)
    Q_ENUMS(CompType)


public:
    explicit DataControl(QObject *parent = 0);
    ~DataControl();

    void registerTypes() const;


    /************************************************************
     * Signals
     ************************************************************/
signals:
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
    void createComponent(const QVariantMap &object);
    bool createChannel(const QVariantMap &object);
    bool clearNetwork();
    void writeLog(QString message,QColor color=Qt::black);

    /************************************************************
     * Slots
     ************************************************************/
public slots:
    bool fileOpen(QUrl fileUrl);
    bool componentCreated(const QVariant &object);
    bool componentDestroyed(const QVariant &object);
    bool componentChanged(const QVariant &object);
    bool channelCreated(const QVariant &object);
    bool channelDestroyed(const QVariant &object);
    bool channelChanged(const QVariant &object);

    /************************************************************
     * Public methods
     ************************************************************/
public:
    virtual XMASComponent *createXmasComponent(QString name, QString type);
    /************************************************************
     * Private methods
     ************************************************************/
private:
    bool emitNetwork(XCompMap &componentMap);
    void convertToQml(QVariantMap &map, XMASComponent *comp);
    void connectInQml(QVariantList &list, XMASComponent *comp);
    bool connectPorts(XMASComponent *comp, QObject *qobject);
    bool createCanvasData(XMASComponent *comp, QObject *qobject);
    /************************************************************
     * Data members
     ************************************************************/
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

private:

    XMP m_mp;
    /**
     * @brief allComponents The internal structure containing the network
     *
     */
    XCompMap m_componentMap;
    Logger m_logger;

    /************************************************************
     * enums and constant data members
     ************************************************************/
    //TODO : enumeration in javascript.
    std::map<std::type_index, CompType> m_type_index_map = {
        {std::type_index(typeid(XMASSource)), Source},
        {std::type_index(typeid(XMASSink)), Sink},
        {std::type_index(typeid(XMASFunction)), Function},
        {std::type_index(typeid(XMASQueue)), Queue},
        {std::type_index(typeid(XMASJoin)), Join},
        {std::type_index(typeid(XMASMerge)), Merge},
        {std::type_index(typeid(XMASFork)), Fork},
        {std::type_index(typeid(XMASSwitch)), Switch},
    };
    // TODO: What to do with IN and OUT????
    std::map<QString, std::type_index> m_type_map = {
        {xsource , std::type_index(typeid(XMASSource)) },
        {xsink, std::type_index(typeid(XMASSink)) },
        {xfunction, std::type_index(typeid(XMASFunction)) },
        {xqueue, std::type_index(typeid(XMASQueue)) },
        {xjoin, std::type_index(typeid(XMASJoin)) },
        {xmerge, std::type_index(typeid(XMASMerge)) },
        {xfork, std::type_index(typeid(XMASFork)) },
        {xswitch, std::type_index(typeid(XMASSwitch)) },
    };
    const char *m_modelName = "XMAS.model";
    const int m_modelMajor = 1;
    const int m_modelMinor = 0;

};

// Remark; copied from parse.cpp
template <class T>
T *insert(bitpowder::lib::MemoryPool& mp,
          std::map<bitpowder::lib::String, XMASComponent*>& allComponents,
          const bitpowder::lib::String& name) {
    //std::cout << name << std::endl;
    if (allComponents.find(name) != allComponents.end())
        throw bitpowder::lib::Exception(42, __FILE__, __LINE__);
    T *comp = new(mp, &bitpowder::lib::destroy<XMASComponent>) T(name);
    allComponents.insert(std::make_pair(comp->getName(), comp));
    return comp;
}


#endif // DATACONTROL_H

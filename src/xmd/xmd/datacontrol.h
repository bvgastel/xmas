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
    void writeLog(QString message,QColor color=Qt::black);

    /************************************************************
     * Slots
     ************************************************************/
public slots:
    bool fileOpen(QUrl fileUrl);

    /************************************************************
     * Public methods
     ************************************************************/
public:
    /************************************************************
     * Private methods
     ************************************************************/
private:
    bool emitNetwork(XCompMap &componentMap);
    void convertToQml(QVariantMap &map, XMASComponent *comp);
    void connectInQml(QVariantList &list, XMASComponent *comp);
    /************************************************************
     * Data members
     ************************************************************/
public:

private:

    XMP m_mp;
    Logger m_logger;

    /************************************************************
     * enums and constant data members
     ************************************************************/
    //TODO : enumeration in javascript.
    std::map<std::type_index, QString> m_type_index_map = {
        {std::type_index(typeid(XMASSource)), xsource},
        {std::type_index(typeid(XMASSink)), xsink},
        {std::type_index(typeid(XMASFunction)), xfunction},
        {std::type_index(typeid(XMASQueue)), xqueue},
        {std::type_index(typeid(XMASJoin)), xjoin},
        {std::type_index(typeid(XMASMerge)), xmerge},
        {std::type_index(typeid(XMASFork)), xfork},
        {std::type_index(typeid(XMASSwitch)), xswitch},
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
    if (allComponents.find(name) != allComponents.end())
        throw bitpowder::lib::Exception(42, __FILE__, __LINE__);
    T *comp = new(mp, &bitpowder::lib::destroy<XMASComponent>) T(name);
    allComponents.insert(std::make_pair(comp->getName(), comp));
    return comp;
}


#endif // DATACONTROL_H

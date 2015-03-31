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
#include "model/component.h"
#include "model/network.h"
#include "xmasproject.h"


const model::Component::CompType xsource = model::Component::CompType::Source;
const model::Component::CompType xsink = model::Component::CompType::Sink;
const model::Component::CompType xfunction = model::Component::CompType::Function;
const model::Component::CompType xqueue = model::Component::CompType::Queue;
const model::Component::CompType xjoin = model::Component::CompType::Join;
const model::Component::CompType xmerge = model::Component::CompType::Merge;
const model::Component::CompType xfork = model::Component::CompType::Fork;
const model::Component::CompType xswitch = model::Component::CompType::Switch;

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

    bool addComponent(model::Component *component);
    bool addComposite(model::Component *component, XMASNetwork &network);

    /************************************************************
     * Public methods
     ************************************************************/
public:
    /************************************************************
     * Private methods
     ************************************************************/
private:
    bool emitNetwork(XMASNetwork &network);
    void convertToQml(QVariantMap &map, XMASComponent *comp);
    void connectInQml(QVariantList &list, XMASComponent *comp);
    /************************************************************
     * Data members
     ************************************************************/
public:

    std::shared_ptr<XMASProject> project();

private:

    Logger m_logger;
    std::shared_ptr<XMASProject> m_project;

    QVariantList m_compositeLibrary;

    /************************************************************
     * enums and constant data members
     ************************************************************/
    //TODO : enumeration in javascript.
    std::map<std::type_index, model::Component::CompType> m_type_index_map = {
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


#endif // DATACONTROL_H

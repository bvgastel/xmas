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

#include <iostream>
#include <map>
#include <set>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>

#include <QQmlContext>
#include <QDebug>
#include <QtQml>
#include <QVariant>
#include "canvascomponentextension.h"
#include "model/network.h"
#include "model/component.h"
#include "model/port.h"
#include "model/channel.h"
#include "export.h"
#include "parse.h"
#include "datacontrol.h"

DataControl::DataControl(QObject *parent) : QObject(parent),m_logger("datacontrol")
{
    QObject::connect(&m_logger, &Logger::writeLog, this, &DataControl::writeLog );
}

DataControl::~DataControl()
{

}

void DataControl::registerTypes() const{
    qmlRegisterType<model::Network>("XMAS.model", 1, 0, "XNetwork");
    qRegisterMetaType<model::Network*>("XNetwork*");
    qmlRegisterType<model::Component>("XMAS.model", 1, 0, "XComponent");
    qmlRegisterType<model::Channel>("XMAS.model", 1, 0, "XChannel");
    qmlRegisterType<model::XPort>("XMAS.model", 1, 0, "XPort");
    qRegisterMetaType<model::XPort*>("XPort*");
}

bool DataControl::fileOpen(QUrl fileUrl) {

    std::string filename =
            fileUrl.isLocalFile() ? fileUrl.toLocalFile().toStdString()
                                  : fileUrl.fileName().toStdString();

    m_logger.log("Opening file " + filename);

    XCompMap componentMap;
    std::tie(componentMap, std::ignore) = parse_xmas_from_file(filename, m_mp);

    if (componentMap.empty()) {
        m_logger.log("[Component.cpp/fileOpen(fileUrl)] File "+ filename + " was parsed as empty. Maybe the file is invalid json input.",Qt::red);
        return false;
    }
    auto result = emitNetwork(componentMap);
    return result;
}

bool DataControl::fileSave(QUrl fileUrl) {

    std::string filename =
            fileUrl.isLocalFile() ? fileUrl.toLocalFile().toStdString()
                                  : fileUrl.fileName().toStdString();


    return false;

}

bool DataControl::emitNetwork(XCompMap &componentMap) {
    // Works with (local) the parameter componentMap,  not the datamember m_componentMap.
    std::clock_t c_start = std::clock();
    QVariantList compList;
    for(auto &it : componentMap) {
        XMASComponent *comp = it.second;
        if (comp) {
            QVariantMap map;
            convertToQml(map, comp);
            compList.append(map);
        }
    }
    QVariantList channelList;
    for (auto &it : componentMap) {
        XMASComponent *comp = it.second;
        if (comp) {
            QVariantList list;
            connectInQml(list, comp);
            channelList.append(list);
        }
    }
    QVariantMap network;
    network["complist"] = compList;
    network["channellist"] = channelList;

    emit createNetwork(network);
    std::clock_t c_end = std::clock();
        qDebug() << "CPU time used: " << 1000.0 * (c_end-c_start) / CLOCKS_PER_SEC << " ms"
                       << " for " << compList.length() << " components and " << channelList.length() << " channels";

    return true;
}

void DataControl::connectInQml(QVariantList &list, XMASComponent *comp) {
    for (Output *out : comp->outputPorts()) {
        if (out->isConnected()) {
            QVariantMap map;
            map.insert("initiator", QString(out->getInitiator()->getStdName().c_str()));
            map.insert("initiatorport", QString(out->getName()));
            map.insert("target", QString(out->getTarget()->getStdName().c_str()));
            map.insert("targetport", QString(out->getTargetPort()->getName()));
//            m_logger.log(
//             "channel created from " + out->getInitiator()->getStdName() +
//             " to " + out->getTarget()->getStdName());
            list.append(map);
        } else {
            m_logger.log("output port " + std::string(out->getName()) + " in comp "
                      + out->getComponent()->getStdName() + " is not connected");
        }
    }
}


void DataControl::convertToQml(QVariantMap &map, XMASComponent *comp) {
    std::string name = comp->getStdName();
//    m_logger.log("name = "+ name + " slot for creation called", Qt::darkGreen);

    std::type_index typeIndex = std::type_index(typeid(*comp));
    QString type = m_type_index_map[typeIndex];
    QString qname = QString::fromStdString(name);

    CanvasComponentExtension *ext = comp->getExtension<CanvasComponentExtension *>();
    if (ext) {
        map.insert("x", ext->x());
        map.insert("y", ext->y());
        map.insert("rotation", ext->orientation());
        map.insert("scale", ext->scale());
    }

    if (type == xqueue) {
        XMASQueue *queue = dynamic_cast<XMASQueue *>(comp);
        QString expression = QString();
        if (queue) {
            int size = queue->c;
            expression = QString(size);
        }
        map.insert("expression", expression);
    } else if (type == xfunction) {
        XMASFunction *func = dynamic_cast<XMASFunction *>(comp);
        QString expression = QString();
        if (func) {
            expression = QString(func->getFunctionExpression(m_mp).stl().c_str());
        }
        map.insert("expression", expression);
    } else if (type == xsource) {
        XMASSource *source = dynamic_cast<XMASSource *>(comp);
        QString expression = QString();
        if (source) {
            expression = QString(source->getSourceExpression(m_mp).stl().c_str());
        }
        map.insert("expression", expression);
    } else if (type == xjoin) { // NOTE: for now only works for restricted join
        XMASJoin *join = dynamic_cast<XMASJoin *>(comp);
        QString expression = QString();
        if (join) {
            expression = QString(join->getJoinExpression(m_mp).stl().c_str());
        }
        map.insert("expression", expression);
    } else if (type == xswitch) {
        XMASSwitch *sw = dynamic_cast<XMASSwitch *>(comp);
        QString expression = QString();
        if (sw) {
            expression = QString(sw->getSwitchExpression(m_mp).stl().c_str());
        }
        map.insert("expression", expression);
    }
    map.insert("type", type);
    map.insert("name", qname);
}

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
#include "composite-network-extension.h"
#include "model/network.h"
#include "model/component.h"
#include "model/port.h"
#include "model/channel.h"
#include "export.h"
#include "parse.h"
#include "datacontrol.h"

DataControl::DataControl(QObject *parent)
    : QObject(parent),
      m_logger("datacontrol"),
      m_project(new XMASProject)
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

std::shared_ptr<XMASProject> DataControl::project() {
    return m_project;
}

bool DataControl::fileOpen(QUrl fileUrl) {

    std::string filename =
            fileUrl.isLocalFile() ? fileUrl.toLocalFile().toStdString()
                                  : fileUrl.fileName().toStdString();

    m_logger.log("Opening file " + filename);

    try {
        m_project.reset(new XMASProject {filename});
    } catch (bitpowder::lib::Exception) {
        m_logger.log("[DataControl::fileOpen] Unable to parse file " + filename + ". Maybe the file is invalid json input.", Qt::red);
        return false;
    }
    auto result = emitNetwork(*m_project->getRootNetwork());
    return result;
}

bool DataControl::addComponent(model::Component *component) {

    if (!project()) {
        emit writeLog(QString("Project not existing! All will fail!"));
        return false;
    }

    std::string name = component->getName().toStdString();
    model::Component::CompType type = component->getType();

    switch(type) {
    case model::Component::CompType::Source :
        m_project->insert<XMASSource>(name);
        break;
    case model::Component::CompType::Sink :
        m_project->insert<XMASSink>(name);
        break;
    case model::Component::CompType::Function :
        m_project->insert<XMASFunction>(name);
        break;
    case model::Component::CompType::Queue :
        m_project->insert<XMASQueue>(name);
        break;
    case model::Component::CompType::Join :
        m_project->insert<XMASJoin>(name);
        break;
    case model::Component::CompType::Merge :
        m_project->insert<XMASMerge>(name);
        break;
    case model::Component::CompType::Switch :
        m_project->insert<XMASSwitch>(name);
        break;
    case model::Component::CompType::Fork :
        m_project->insert<XMASFork>(name);
        break;
    case model::Component::CompType::Composite :
        emit writeLog(QString("type composite is not implemented .... yet"), Qt::red);
        return false;
    default :
        emit writeLog(QString("Unknown component type!"), Qt::red);
        return false;
    }

    return true;
}

bool DataControl::addCompositeNetwork(QUrl url) {

    std::string name = url.toLocalFile().toStdString();
    XMASNetwork* xmas_network = m_project->loadNetwork(name);


    auto cne = xmas_network->getNetworkExtension<CompositeNetworkExtension>(false);
    if (!cne)
        return false;   // Composite network information missing, can't use as a composite network!

    QVariantMap map;
    map.insert("url", url);
    map.insert("alias", QString::fromStdString(cne->alias));
    map.insert("symbol", QString::fromStdString(cne->imageName));
    map.insert("boxed", cne->boxedImage);
    map.insert("xmas_network", qVariantFromValue((void*)xmas_network));
    m_compositeLibrary.append(map);

    return true;
}

bool DataControl::emitNetwork(XMASNetwork &network) {

    auto& components = network.getComponentMap();

    std::clock_t c_start = std::clock();
    QVariantList compList;
    for(auto &it : components) {
        XMASComponent *comp = it.second;
        if (comp) {
            QVariantMap map;
            convertToQml(map, comp);
            compList.append(map);
        }
    }
    QVariantList channelList;
    for (auto &it : components) {
        XMASComponent *comp = it.second;
        if (comp) {
            QVariantList list;
            connectInQml(list, comp);
            channelList.append(list);
        }
    }
    QVariantMap qmlNetwork;
    qmlNetwork["complist"] = compList;
    qmlNetwork["channellist"] = channelList;

    emit createNetwork(qmlNetwork);
    std::clock_t c_end = std::clock();
        qDebug() << "CPU time used: " << 1000.0 * (c_end-c_start) / CLOCKS_PER_SEC << " ms"
                       << " for " << compList.length() << " components and " << channelList.length() << " channels";

    return true;
}

void DataControl::connectInQml(QVariantList &list, XMASComponent *comp) {
    for (Output *out : comp->outputPorts()) {
        if (out->isConnected()) {
            QVariantMap map;
            map.insert("outcomp", QString(out->getInitiator()->getStdName().c_str()));
            map.insert("outport", QString(out->getName()));
            map.insert("incomp", QString(out->getTarget()->getStdName().c_str()));
            map.insert("inport", QString(out->getTargetPort()->getName()));
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
    model::Component::CompType type = m_type_index_map[typeIndex];
    QString qname = QString::fromStdString(name);

    CanvasComponentExtension *ext = comp->getExtension<CanvasComponentExtension *>();
    if (ext) {
        map.insert("x", ext->x());
        map.insert("y", ext->y());
        map.insert("rotation", ext->orientation());
        map.insert("scale", ext->scale());
    }

    bitpowder::lib::MemoryPool& mp = m_project->mp();

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
            expression = QString(func->getFunctionExpression(mp).stl().c_str());
        }
        map.insert("expression", expression);
    } else if (type == xsource) {
        XMASSource *source = dynamic_cast<XMASSource *>(comp);
        QString expression = QString();
        if (source) {
            expression = QString(source->getSourceExpression(mp).stl().c_str());
        }
        map.insert("expression", expression);
    } else if (type == xjoin) { // NOTE: for now only works for restricted join
        XMASJoin *join = dynamic_cast<XMASJoin *>(comp);
        QString expression = QString();
        if (join) {
            expression = QString(join->getJoinExpression(mp).stl().c_str());
        }
        map.insert("expression", expression);
    } else if (type == xswitch) {
        XMASSwitch *sw = dynamic_cast<XMASSwitch *>(comp);
        QString expression = QString();
        if (sw) {
            expression = QString(sw->getSwitchExpression(mp).stl().c_str());
        }
        map.insert("expression", expression);
    }
    map.insert("type", type);
    map.insert("name", qname);
}

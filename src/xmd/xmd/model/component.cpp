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

#include "component.h"

model::Component::Component(QQuickItem *parent)
    : QQuickItem(parent)
{
    m_inputports = QList<XPort *>();
    m_outputports = QList<XPort *>();
    m_component = nullptr;
    qDebug() << "Constructor component done.";
}

model::Component::~Component()
{
    delete m_component;
}

void model::Component::classBegin() {
    // No action needed.
}

/**
 * @brief model::Component::componentComplete
 *
 * When the QML properties for Component are filled completely,
 * we can create the component and all of its ports.
 *
 */
void model::Component::componentComplete() {
    m_type = (CompType)this->property("type").toInt();
    m_component = createComponent(m_type, m_name);
    if(m_component) {
        emitInports();
        emitOutports();
    }
}

void model::Component::emitInports()
{
    // ####################################################################
    // To decide: just deliver data and let qml create xport
    //            QVariantList with properties name and type
    //            no component is connected on creation.
    QVariantList list;
    for (Port *p : m_component->inputPorts()) {
        QVariantMap map;
        map.insert("name", QString(p->getName));
        map.insert("type", XPort::PortType::INPORT);
        list.append(map);
    }
    // emit inputPortsChanged(list);  --> signature not yet adapted
    // End proposal: return QVariantList through signal inputPortsChanged()
    // ####################################################################
    m_inputports.clear();
    for (Port* p : m_component->inputPorts())
    {
        XPort *xport = new XPort();
        xport->setName(p->getName());
        xport->setType(XPort::PortType::INPORT);
        xport->setConnected(p->isConnected());  // necessary? Only emits.
        m_inputports.append(xport);
        qDebug() << "inportname = " << xport->getName();

    }
    emit inputPortsChanged();

}

void model::Component::emitOutports()
{
    m_outputports.clear();
    for (Port* p : m_component->outputPorts())
    {
        XPort *xport = new XPort();
        xport->setName(p->getName());
        xport->setType(XPort::PortType::OUTPORT);
        xport->setConnected(p->isConnected()); // necessary? Only emits.
        m_outputports.append(xport);
        qDebug() << "outportname = " << xport->getName();

    }
    emit outputPortsChanged();
}

XMASComponent *model::Component::createComponent(CompType type, QString qname) {
    XMASComponent *component = nullptr;
    std::string name = qname.toStdString();
    switch(type) {
    case CompType::Source :
        component = new XMASSource(name);
        break;
    case CompType::Sink :
        component = new XMASSink(name);
        break;
    case CompType::Function :
        component = new XMASFunction(name);
        break;
    case CompType::Queue :
        component = new XMASQueue(name);
        break;
    case CompType::Join :
        component = new XMASJoin(name);
        break;
    case CompType::Merge :
        component = new XMASMerge(name);
        break;
    case CompType::Switch :
        component = new XMASSwitch(name);
        break;
    case CompType::Fork :
        component = new XMASFork(name);
        break;
    case CompType::In :
        emit writeLog(QString("type in is not implemented .... yet"), Qt::red);
        break;
    case CompType::Out :
        emit writeLog(QString("type out is not implemented .... yet"), Qt::red);
        break;
    case CompType::Composite :
        emit writeLog(QString("type composite is not implemented .... yet"), Qt::red);
        break;
    default :
        emit writeLog(QString("Unknown component type!"), Qt::red);
    }
    return component;
}

//TODO Check if name is unique and not empty : return -1 if ok
int model::Component::checkName(QString name) {
    bitpowder::lib::unused(name);
    return -1;
}

// TODO: Update in XMASComponent
// TODO check expression en emit valid changed with -1 if ok , or > -1 if not where int is position error
int model::Component::updateExpression(QVariant expression) {
    QString typeName = QString(expression.typeName());
    emit writeLog(QString("[debug] received expression of type '")+typeName+"' and contents "+expression.toString());

    if (getType() == Queue) {
        if (typeName != "int") {
            setValidExpr(false, 0, QString("Received non integer size."));
            return 0;
        }
        m_expression = expression;
        if (!m_component) { // if we are still constructing Component.
            return 0;
        }
        int size = expression.toInt();
        XMASQueue *queue = dynamic_cast<XMASQueue *>(this->m_component);
        if (!queue) {
            emit writeLog(QString("Fatal error in Component: "
                             "did not recognize m_component as queue."));
            throw bitpowder::lib::Exception("Fatal error in Component.");
        }
        queue->c = size;
        setValidExpr(true, -1, QString(""));
        emit writeLog(QString("queuesize set: ")+size);
        return -1;
    } else if (getType() == Source) {
        if (typeName != "QString") {
            return 0;
        }
        m_expression = expression;
        QString qexpr = expression.toString();
        XMASSource *source = dynamic_cast<XMASSource *>(this->m_component);
        if (source) {
            std::string expr = qexpr.toStdString();
            auto result = source->setSourceExpression(expr, m_mp);
            QString errMsg = QString(result.m_errMsg.stl().c_str());
            setValidExpr(result.m_success, result.m_pos, errMsg);
            QString xmas_expression = result.m_success ? QString(source->getSourceExpression(m_mp).stl().c_str())
                                                       : "<should never show>";
            emit writeLog(QString("saving expression in XMASComponent ")
                     + (result.m_success? "succeeded." + xmas_expression
                                        : "failed. Error message is:" + errMsg));
            return result.m_pos;
        } else {
            emit writeLog(QString("Fatal error in Component: "
                             "did not recognize m_component as source."));
            throw bitpowder::lib::Exception("Fatal error in Component.");
        }
    } else if (getType() == Function) {
        if (typeName != "QString") {
            return 0;
        }
        m_expression = expression;
        QString qexpr = expression.toString();
        XMASFunction *func = dynamic_cast<XMASFunction *>(this->m_component);
        if (func) {
            std::string expr = qexpr.toStdString();
            auto result =  func->setFunctionExpression(expr, m_mp);
            QString errMsg = QString(result.m_errMsg.stl().c_str());
            setValidExpr(result.m_success, result.m_pos, errMsg);
            emit writeLog(QString("saving expression in XMASComponent ")
                     + (result.m_success? "succeeded." : "failed. Error message is:" + errMsg));
            if (result.m_success) {
                QString xmas_expression = QString(func->getFunctionExpression(m_mp).stl().c_str());
                emit writeLog(QString("result = ") + xmas_expression );
            }
            return result.m_pos;
        } else {
            emit writeLog(QString("Fatal error in Component: "
                             "did not recognize m_component as function."));
            throw bitpowder::lib::Exception("Fatal error in Component.");
        }
        return true;
    } else if (getType() == Join) {
        if (typeName != "QString") {
            return 0;
        }
        m_expression = expression;
        QString qexpr = expression.toString();
        XMASJoin *join = dynamic_cast<XMASJoin *>(this->m_component);
        if (join) {
            std::string expr = qexpr.toStdString();
            ExpressionResult result;
            QString kindOfJoin;
            if (expr == "0" || expr == "1") {
                kindOfJoin = "restricted";
                result = join->setRestrictedJoinPort(expr, m_mp);
            } else {
                kindOfJoin = "unrestricted";
                result =  join->setUnrestrictedJoinExpression(expr, m_mp);
            }
            QString errMsg = QString(result.m_errMsg.stl().c_str());
            setValidExpr(result.m_success, result.m_pos, errMsg);
            QString xmas_expression = result.m_success ? QString(join->getJoinExpression(m_mp).stl().c_str())
                                                       : "<Should never show>";
            xmas_expression = xmas_expression == "" ? "<no value returned>" : xmas_expression;
            emit writeLog(QString("saving ")+ kindOfJoin + QString(" join expression in XMASComponent ")
                     + (result.m_success? "succeeded. Stored \"" + xmas_expression + "\""
                                        : "failed. Error message is:" + errMsg));
            emit writeLog(QString("Function for Join is not implemented yet."));
            return result.m_pos;
        } else {
            emit writeLog(QString("Fatal error in Component: "
                             "did not recognize m_component as switch."));
            throw bitpowder::lib::Exception("Fatal error in Component.");
        }
        return true;
    } else if (getType() == Switch) {
        if (typeName != "QString") {
            return 0;
        }
        m_expression = expression;
        QString qexpr = expression.toString();
        XMASSwitch *sw = dynamic_cast<XMASSwitch *>(this->m_component);
        if (sw) {
            std::string expr = qexpr.toStdString();
            auto result =  sw->setSwitchExpression(expr, m_mp);
            QString errMsg = QString(result.m_errMsg.stl().c_str());
            setValidExpr(result.m_success, result.m_pos, errMsg);
            QString xmas_expression = result.m_success ? QString(sw->getSwitchExpression(m_mp).stl().c_str())
                                                       : "<Should never show>";
            emit writeLog(QString("saving expression in XMASComponent ")
                     + (result.m_success? "succeeded." + xmas_expression
                                        : "failed. Error message is:" + errMsg));
            return result.m_pos;
        } else {
            emit writeLog(QString("Fatal error in Component: "
                             "did not recognize m_component as switch."));
            throw bitpowder::lib::Exception("Fatal error in Component.");
        }
        return true;
    }

    return false;
}

QVariant model::Component::getExpression() {
    // In case we are not finished constructing
    if (!m_component) {
        return QVariant();
    }
    // In case of queue return queue size
    auto queue = dynamic_cast<XMASQueue *>(m_component);
    if (queue) {
        qulonglong expr = queue->c;
        return expr;
    }
    // In case of function return function specification.
    auto func = dynamic_cast<XMASFunction *>(m_component);
    if (func) {
        auto expr = func->getFunctionExpression(m_mp).stl();
        if (expr != "") {
            return QString(expr.c_str()); // Only return xmas string, if useful
        } else {
            return m_expression;
        }
    }
    // In case of source return source specification.
    auto src = dynamic_cast<XMASSource *>(m_component);
    if (src) {
        auto expr = src->getSourceExpression(m_mp);
        if (expr != "") {
            return QString(expr.stl().c_str());  // Only return xmas string, if useful
        } else {
            return m_expression;
        }
    }
    auto sw = dynamic_cast<XMASSwitch *>(m_component);
    if (sw) {
        auto expr = sw->getSwitchExpression(m_mp);
        if (expr != "") {
            return QString(expr.stl().c_str());  // Only return xmas string, if useful
        } else {
            return m_expression;
        }
    }
    auto join = dynamic_cast<XMASJoin *>(m_component);
    if (join) {
        auto expr = join->getJoinExpression(m_mp);
        if (expr != "") {
            return QString(expr.stl().c_str());  // Only return xmas string, if useful
        } else {
            return m_expression;
        }
    }
    return m_expression;
}

/************************************************************************************
 * Inports
 ************************************************************************************/
/**
 * @brief model::Component::getInputPorts
 * @return a qml list of XPort instances
 */
QQmlListProperty<model::XPort> model::Component::getInputPorts()
{
    return QQmlListProperty<model::XPort>(this, 0,
                                               &model::Component::append_inport_list,
                                               &model::Component::count_inport_list,
                                               &model::Component::at_inport_list,
                                               &model::Component::clear_inport_list);

}

void model::Component::append_inport_list(QQmlListProperty<model::XPort> *property, model::XPort *port) {
    Component *component = qobject_cast<Component *>(property->object);
    if (component) {
        port->setParent(component);
        component->m_inputports.append(port);
    }
}

int model::Component::count_inport_list(QQmlListProperty<model::XPort> *property) {
    Component *component = qobject_cast<Component *>(property->object);
    if (component) {
        return component->m_inputports.size();
    }
    //TODO: Should we emit an error signal? How? There is no instance! (static)
    return 0;
}

model::XPort *model::Component::at_inport_list(QQmlListProperty<model::XPort> *property, int index) {
    if (property->data) {
        auto *list = static_cast< QList<model::XPort *> *>(property->data);
        model::XPort *port = list->at(index);
        return port;
    }
    return nullptr;
}

void model::Component::clear_inport_list(QQmlListProperty<model::XPort> *property) {
    Component *component = qobject_cast<Component *>(property->object);
    if (component) {
        component->m_inputports.clear();
    }
}

/************************************************************************************
 * Outports
 ************************************************************************************/
/**
 * @brief model::Component::getOutputPorts
 * @return
 */
QQmlListProperty<model::XPort> model::Component::getOutputPorts()
{
    return QQmlListProperty<model::XPort>(this, 0,
                                               &model::Component::append_outport_list,
                                               &model::Component::count_outport_list,
                                               &model::Component::at_outport_list,
                                               &model::Component::clear_outport_list);
}

void model::Component::append_outport_list(QQmlListProperty<XPort> *property, XPort *port) {
    Component *component = qobject_cast<Component *>(property->object);
    if (component) {
        port->setParent(component);
        component->m_outputports.append(port);
    }
}

int model::Component::count_outport_list(QQmlListProperty<XPort> *property) {
    Component *component = qobject_cast<Component *>(property->object);
    if (component) {
        return component->m_outputports.size();
    }
    //TODO: Should we emit an error signal? How? There is no instance! (static)
    return 0;
}

model::XPort *model::Component::at_outport_list(QQmlListProperty<XPort> *property, int index) {
    if (property->data) {
        auto *list = static_cast< QList<model::XPort *> *>(property->data);
        model::XPort *port = list->at(index);
        return port;
    }
    return nullptr;
}

void model::Component::clear_outport_list(QQmlListProperty<XPort> *property) {
    Component *component = qobject_cast<Component *>(property->object);
    if (component) {
        component->m_outputports.clear();
    }
}




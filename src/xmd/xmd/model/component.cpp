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

}

model::Component::~Component()
{
    delete m_component;
}

void model::Component::classBegin() {
    // No action needed.
}

void model::Component::componentComplete() {
    m_type = (CompType)this->property("type").toInt();
    m_component = createComponent(m_type, m_name);
    extractPorts();
}

// TODO: What to do with IN and OUT?
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
    writeLog(QString("ontvangen expressie heeft type '")+typeName+"' en inhoud "+expression.toString());

    if (getType() == Queue) {
        if (typeName != "int") {
            setValidExpr(false, 0, QString("Received non integer size."));
            return 0;
        }
        int size = expression.toInt();
        XMASQueue *queue = dynamic_cast<XMASQueue *>(this->m_component);
        if (!queue) {
            writeLog(QString("Fatal error in Component: "
                             "did not recognize m_component as queue."));
            throw bitpowder::lib::Exception("Fatal error in Component.");
        }
        queue->c = size;
        m_expression = expression;
        setValidExpr(true, -1, QString(""));
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
            writeLog(QString("saving expression in XMASComponent ")
                     + (result.m_success? "succeeded." : "failed. Error message is:"));
            if (!result.m_success) {
                writeLog(errMsg);
            }
            return result.m_pos;
        } else {
            writeLog(QString("Fatal error in Component: "
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
            writeLog(QString("saving expression in XMASComponent ")
                     + (result.m_success? "succeeded." : "failed. Error message is:" + errMsg));
            return result.m_pos;
        } else {
            writeLog(QString("Fatal error in Component: "
                             "did not recognize m_component as source."));
            throw bitpowder::lib::Exception("Fatal error in Component.");
        }
        return true;
    } else if (getType() == Join) {
        // join
        // TODO: expressies? waarschijnlijk / unrestricted
        return true;
    } else if (getType() == Switch) {
        // switch
        // TODO: expressie?
        return true;
    }

    return false;
}


//###################################################################################################
//TODO : to be reviewed with Guus
/**
 * @brief model::Component::getPorts
 * @return
 */
QQmlListProperty<model::XPort> model::Component::getPorts()
{
    return QQmlListProperty<model::XPort>(this,m_ports);
}

/**
 * @brief model::Component::append_port
 * @param list
 * @param port
 */
void model::Component::append_port(QQmlListProperty<model::XPort> *list, model::XPort *port)
{
    Component *component = qobject_cast<Component *>(list->object);
    if (port)
        component->m_ports.append(port);
}
// Port here must be the xmascomponent port type!!
void model::Component::extractPorts(void)
{
    for (Port* p : m_component->inputPorts())
    {
        XPort *xport = new XPort();
        xport->setName(p->getName());
        xport->setType(XPort::PortType::Target);
        m_ports.append(xport);
        qDebug() << "inpoortnaam = " << xport->getName();

    }
    for (Port* p : m_component->outputPorts())
    {
        XPort *xport = new XPort();
        xport->setName(p->getName());
        xport->setType(XPort::PortType::Initiator);
        m_ports.append(xport);
        qDebug() << "outpoortnaam = " << xport->getName();

    }
}
//###################################################################################################



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
    //###################################################################################################
    //TODO : to be reviewed with Guus
    if(m_component) {
        extractPorts();
    }
    //###################################################################################################
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
    emit writeLog(QString("[debug]ontvangen expressie heeft type '")+typeName+"' en inhoud "+expression.toString());

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


//###################################################################################################
//TODO : to be reviewed with Guus
/**
 * @brief model::Component::getInputPorts
 * @return
 */
QQmlListProperty<model::XPort> model::Component::getInputPorts()
{
    return QQmlListProperty<model::XPort>(this,m_inputports);
}

/**
 * @brief model::Component::getOutputPorts
 * @return
 */
QQmlListProperty<model::XPort> model::Component::getOutputPorts()
{
    return QQmlListProperty<model::XPort>(this,m_outputports);
}

///**
// * @brief model::Component::append_port
// * @param list
// * @param port
// */
//void model::Component::append_port(QQmlListProperty<model::XPort> *list, model::XPort *port)
//{
//    Component *component = qobject_cast<Component *>(list->object);
//    if (port)
//        component->m_ports.append(port);
//}
// Port here must be the xmascomponent port type!!
void model::Component::extractPorts(void)
{
    m_inputports.clear();
    for (Port* p : m_component->inputPorts())
    {
        XPort *xport = new XPort();
        xport->setName(p->getName());
        xport->setType(XPort::PortType::INPORT);
        xport->setConnected(p->isConnected());
        m_inputports.append(xport);
        qDebug() << "inpoortnaam = " << xport->getName();

    }
    emit inputPortsChanged();

    m_outputports.clear();
    for (Port* p : m_component->outputPorts())
    {
        XPort *xport = new XPort();
        xport->setName(p->getName());
        xport->setType(XPort::PortType::OUTPORT);
        xport->setConnected(p->isConnected());
        m_outputports.append(xport);
        qDebug() << "outpoortnaam = " << xport->getName();

    }
    emit outputPortsChanged();
}
//###################################################################################################



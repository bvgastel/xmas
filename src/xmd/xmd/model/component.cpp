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

#include "datacontrol.h"
#include "component.h"

extern DataControl *dataControl;

model::Component::Component(QQuickItem *parent)
    : QQuickItem(parent)
{
}

model::Component::~Component()
{
}

void model::Component::classBegin() {
}

/**
 * @brief model::Component::componentComplete
 *
 * When the QML properties for Component are filled completely,
 * Currently no action necessary.
 *
 */
void model::Component::componentComplete() {
    // no action.
}

XMASComponent *model::Component::createXMASComponent(CompType type, QString qname) {
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

    auto c = xmas_component();
    if (getType() == Queue) {
        if (typeName != "int") {
            setValidExpr(false, 0, QString("Received non integer size."));
            return 0;
        }
        m_expression = expression;
        int size = expression.toInt();
        XMASQueue *queue = dynamic_cast<XMASQueue *>(c);
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
        XMASSource *source = dynamic_cast<XMASSource *>(c);
        if (source) {
            std::string expr = qexpr.toStdString();
            auto result = source->setSourceExpression(expr, m_mp);
            QString errMsg = QString(result.m_errMsg.stl().c_str());
            setValidExpr(result.m_success, result.m_pos, errMsg);
            QString xmas_expression =
                    result.m_success ? QString(source->getSourceExpression(m_mp)
                                               .stl().c_str())
                                     : "<should never show>";
            emit writeLog(QString("saving expression in XMASComponent ")
                     + (result.m_success? "succeeded." + xmas_expression
                                        : "failed. Error message is:" + errMsg));
            return result.m_pos;
        } else {
            std::cerr << "Fatal error in Component: did not recognize m_component"
                         " as source." << std::endl;
            throw bitpowder::lib::Exception("Fatal error in Component.");
        }
    } else if (getType() == Function) {
        if (typeName != "QString") {
            return 0;
        }
        m_expression = expression;
        QString qexpr = expression.toString();
        XMASFunction *func = dynamic_cast<XMASFunction *>(c);
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
            std::cerr << "Fatal error in Component: did not recognize m_component"
                         " as function." << std::endl;
            throw bitpowder::lib::Exception("Fatal error in Component.");
        }
        return true;
    } else if (getType() == Join) {
        if (typeName != "QString") {
            return 0;
        }
        m_expression = expression;
        QString qexpr = expression.toString();
        XMASJoin *join = dynamic_cast<XMASJoin *>(c);
        if (join) {
            std::string expr = qexpr.toStdString();
            ExpressionResult result;
            QString kindOfJoin;
            if (expr == "0" || expr == "1") {
                kindOfJoin = "restricted";
                result = join->setRestrictedJoinPort(expr, m_mp);
            } else {
                kindOfJoin = "unrestricted";
                result = join->setUnrestrictedJoinExpression(expr, m_mp);
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
            std::cerr << "Fatal error in Component: did not recognize m_component"
                         " as join." << std::endl;
            throw bitpowder::lib::Exception("Fatal error in Component.");
        }
        return true;
    } else if (getType() == Switch) {
        if (typeName != "QString") {
            return 0;
        }
        m_expression = expression;
        QString qexpr = expression.toString();
        XMASSwitch *sw = dynamic_cast<XMASSwitch *>(c);
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
            std::cerr << "Fatal error in Component: did not recognize m_component"
                         " as switch." << std::endl;
            throw bitpowder::lib::Exception("Fatal error in Component.");
        }
        return true;
    }

    return false;
}

XMASComponent *model::Component::xmas_component() {
    auto project = dataControl->project();
    if (!project) {
        return nullptr;
    }

    auto network = project->getRootNetwork();
    if (!network) {
        return nullptr;
    }

    bitpowder::lib::String name = bitpowder::lib::String(getName().toStdString().c_str());
    auto xmap = network->getComponentMap();
    auto c = xmap[name];
    if (!c) {
        return nullptr;
    }

    return c;
}

QVariant model::Component::getExpression() {
    // gbo: TODO: access xmas_component from XMASNetwork
    auto c = xmas_component();
    // In case of queue return queue size
    auto queue = dynamic_cast<XMASQueue *>(c);
    if (queue) {
        qulonglong expr = queue->c;
        return expr;
    }
    // In case of function return function specification.
    auto func = dynamic_cast<XMASFunction *>(c);
    if (func) {
        auto expr = func->getFunctionExpression(m_mp).stl();
        if (expr != "") {
            return QString(expr.c_str()); // Only return xmas string, if useful
        } else {
            return m_expression;
        }
    }
    // In case of source return source specification.
    auto src = dynamic_cast<XMASSource *>(c);
    if (src) {
        auto expr = src->getSourceExpression(m_mp);
        if (expr != "") {
            return QString(expr.stl().c_str());  // Only return xmas string, if useful
        } else {
            return m_expression;
        }
    }
    auto sw = dynamic_cast<XMASSwitch *>(c);
    if (sw) {
        auto expr = sw->getSwitchExpression(m_mp);
        if (expr != "") {
            return QString(expr.stl().c_str());  // Only return xmas string, if useful
        } else {
            return m_expression;
        }
    }
    auto join = dynamic_cast<XMASJoin *>(c);
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
 * @brief model::Component::ports
 * @return a qml list of XPort instances
 */
QQmlListProperty<model::XPort> model::Component::ports()
{
    return QQmlListProperty<model::XPort>(this, 0,
                                               &model::Component::append_port,0,0,0);

}

void model::Component::append_port(QQmlListProperty<model::XPort> *list, model::XPort *port) {
    Component *component = qobject_cast<Component *>(list->object);
    if (component) {
        port->setParent(component);
        component->m_ports.append(port);
    }
}


//#############################################################################################################
//#############################################################################################################
//##
//##        Example of how to populate a composite component
//##
//##
//#############################################################################################################
//#############################################################################################################



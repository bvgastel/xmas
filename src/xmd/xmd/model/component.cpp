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
#include "port.h"

extern DataControl *dataControl;

model::Component::Component(QQuickItem *parent)
    : QQuickItem(parent), m_name()
{
}

model::Component::~Component()
{
}

void model::Component::updateCanvasData() {
    auto project = dataControl->project();
    auto network = project->getRootNetwork();
    XMASComponent *c = network->getComponent(name().toStdString());
    if (c) {
        c->canvasData(this->x(), this->y(), this->rotation(), this->scale());
    }
}

// This method was made for composite objects.
// Primitives have fixed portnames
QVariantMap model::Component::getPorts()
{
    auto project = dataControl->project();
    if (!project) {
        emit writeLog("[model::Component::getPorts()] no project: big problems.");
        return QVariantMap();
    }

    auto c = xmas_component();
    QVariantMap map;
    if(c){
        for(Port *p : c->ports()) {
            QString name = p->getName();
            map[name] = typeid(*p) == typeid(Input) ? XPort::PortType::INPORT : XPort::PortType::OUTPORT;
        }
    }
    return map;
}

QString model::Component::name() {
    return m_name;
}

void model::Component::name(QString name) {
    // initial setting
    if (m_name == QString()) {
        m_name = name;
        emit nameChanged(true);
        return;
    }
    // change current name
    if (name == m_name) {
        return;
    }
    // get project
    auto project = dataControl->project();
    if (!project) {
        return;
    }
    if (project->changeComponentName(m_name.toStdString(), name.toStdString())) {
        m_name = name;
        emit nameChanged(true);
    }
    emit nameChanged(false);
}

model::Component::CompType model::Component::type() const {
    return m_type;
}

void model::Component::type(CompType type) {
    m_type = type;
    emit typeChanged();
}

bool model::Component::validExpr() {
    return m_validExpr;
}

void model::Component::validExpr(bool validExpr) {
    m_validExpr = validExpr;
    emit validExprChanged(-1, QString(""));
}

void model::Component::validExpr(bool validExpr, int pos, QString errMsg) {
    m_validExpr = validExpr;
    emit validExprChanged(pos, errMsg);
}

void model::Component::classBegin()
{
    // no action
}

void model::Component::componentComplete()
{
    if (m_expression != "") {
        updateExpression();
    }

}

bool model::Component::addXComponent() {
    auto project = dataControl->project();
    bool result = false;
    std::string name = m_name.toStdString();
    try {
        switch(m_type) {
        case model::Component::CompType::Source :
            result = project->insertSource(name);
            break;
        case model::Component::CompType::Sink :
            result = project->insertSink(name);
            break;
        case model::Component::CompType::Function :
            result = project->insertFunction(name);
            break;
        case model::Component::CompType::Queue :
            result = project->insertQueue(name);
            break;
        case model::Component::CompType::Join :
            result = project->insertJoin(name);
            break;
        case model::Component::CompType::Merge :
            result = project->insertMerge(name);
            break;
        case model::Component::CompType::Switch :
            result = project->insertSwitch(name);
            break;
        case model::Component::CompType::Fork :
            result = project->insertFork(name);
            break;
        case model::Component::CompType::Composite :
            result = addComposite();
            break;
        default :
            emit writeLog(QString("Unknown component type!"), Qt::red);
            return false;
        }
    } catch (bitpowder::lib::Exception e) {}      // component already exists, ignore

    if (result) {
        emit componentAdded();
    }

    return result;
}

bool model::Component::addComposite() {

    auto project = dataControl->project();

    QUrl fileUrl = QUrl(property("url").toString());
    std::string filename = fileUrl.fileName().toStdString();
    XMASNetwork *network = project->getNetwork(filename);
    if (!network)
        return false;

    std::string name = m_name.toStdString();
    if(project->insertComposite(name, std::ref(*network))){
        return true;
    }
    return false;
}

bool model::Component::valid() {
    bool result = false;
    auto c = xmas_component();
    if (c) {
        result = c->valid();
    }
    return result;
}

bitpowder::lib::MemoryPool &model::Component::mp() {
    auto project = dataControl->project();
    return project->mp();
}

int model::Component::updateExpression(QVariant expression) {
    m_expression = expression;
    return updateExpression();
}

int model::Component::updateExpression() {
    QString typeName = QString(m_expression.typeName());
    emit writeLog(QString("[debug] received expression of type '")+typeName+"' and contents "+m_expression.toString());

    auto c = xmas_component();
    if (!c) {
        // some calls are before component is complete: without name, no xmas component.
        return false;
    }

    if (type() == Queue) {
        if (typeName != "int") {
            validExpr(false, 0, QString("Received non integer size."));
            return 0;
        }
        int size = m_expression.toInt();
        XMASQueue *queue = dynamic_cast<XMASQueue *>(c);
        if (!queue) {
            emit writeLog(QString("Fatal error in Component: "
                                  "did not recognize m_component as queue."));
            std::cerr   << "Fatal error in Component: did not recognize m_component"
                           " as queue : " << c->getStdName() << std::endl;
            return false;
        }
        queue->c = size;
        validExpr(true, -1, QString(""));
        emit writeLog(QString("queuesize set: ")+size);
        return -1;
    } else if (type() == Source) {
        if (typeName != "QString") {
            return 0;
        }
        QString qexpr = m_expression.toString();
        XMASSource *source = dynamic_cast<XMASSource *>(c);
        if (source) {
            std::string expr = qexpr.toStdString();
            auto result = source->setSourceExpression(expr, mp());
            QString errMsg = QString(result.m_errMsg.stl().c_str());
            validExpr(result.m_success, result.m_pos, errMsg);
            QString xmas_expression =
                    result.m_success ? QString(source->getSourceExpression(mp())
                                               .stl().c_str())
                                     : "<should never show>";
            emit writeLog(QString("saving expression in XMASComponent ")
                          + (result.m_success? "succeeded." + xmas_expression
                                             : "failed. Error message is:" + errMsg));
            return result.m_pos;
        } else {
            std::cerr << "Fatal error in Component: did not recognize m_component"
                         " as source : " << c->getStdName() << std::endl;
            return false;
        }
    } else if (type() == Function) {
        if (typeName != "QString") {
            return 0;
        }
        QString qexpr = m_expression.toString();
        XMASFunction *func = dynamic_cast<XMASFunction *>(c);
        if (func) {
            std::string expr = qexpr.toStdString();
            auto result =  func->setFunctionExpression(expr, mp());
            QString errMsg = QString(result.m_errMsg.stl().c_str());
            validExpr(result.m_success, result.m_pos, errMsg);
            emit writeLog(QString("saving expression in XMASComponent ")
                          + (result.m_success? "succeeded." : "failed. Error message is:" + errMsg));
            if (result.m_success) {
                std::string expr = func->getFunctionExpression(mp());
                QString xmas_expression = QString(expr.c_str());
                emit writeLog(QString("result = ") + xmas_expression );
            }
            return result.m_pos;
        } else {
            std::cerr   << "Fatal error in Component: did not recognize m_component"
                           " as function : "
                        << c->getStdName() << std::endl;
            return false;
        }
        return true;
    } else if (type() == Join) {
        if (typeName != "QString") {
            return 0;
        }
        QString qexpr = m_expression.toString();
        XMASJoin *join = dynamic_cast<XMASJoin *>(c);
        if (join) {
            std::string expr = qexpr.toStdString();
            ExpressionResult result;
            QString kindOfJoin;
            if (expr == "0" || expr == "1") {
                kindOfJoin = "restricted";
                result = join->setRestrictedJoinPort(expr, mp());
            } else {
                kindOfJoin = "unrestricted";
                result = join->setUnrestrictedJoinExpression(expr, mp());
                emit writeLog(QString("Function for unrestricted Join is not implemented yet."));
            }
            QString errMsg = QString(result.m_errMsg.stl().c_str());
            validExpr(result.m_success, result.m_pos, errMsg);
            QString xmas_expression = result.m_success ? QString(join->getJoinExpression(mp()).stl().c_str())
                                                       : "<Should never show>";
            xmas_expression = xmas_expression == "" ? "<no value returned>" : xmas_expression;
            emit writeLog(QString("saving ")+ kindOfJoin + QString(" join expression in XMASComponent ")
                          + (result.m_success? "succeeded. Stored \"" + xmas_expression + "\""
                                             : "failed. Error message is:" + errMsg));
            return result.m_pos;
        } else {
            std::cerr << "Fatal error in Component: did not recognize m_component"
                         " as join : " << c->getStdName() << std::endl;
            return false;
        }
        return true;
    } else if (type() == Switch) {
        if (typeName != "QString") {
            return 0;
        }
        QString qexpr = m_expression.toString();
        XMASSwitch *sw = dynamic_cast<XMASSwitch *>(c);
        if (sw) {
            std::string expr = qexpr.toStdString();
            auto result =  sw->setSwitchExpression(expr, mp());
            QString errMsg = QString(result.m_errMsg.stl().c_str());
            validExpr(result.m_success, result.m_pos, errMsg);
            QString xmas_expression = result.m_success ? QString(sw->getSwitchExpression(mp()).stl().c_str())
                                                       : "<Should never show>";
            emit writeLog(QString("saving expression in XMASComponent ")
                          + (result.m_success? "succeeded." + xmas_expression
                                             : "failed. Error message is:" + errMsg));
            return result.m_pos;
        } else {
            std::cerr << "Fatal error in Component: did not recognize m_component"
                         " as switch : " << c->getStdName() << std::endl;
            return false;
        }
        return true;
    }

    emit writeLog(QString("Component type not recognized for expression: ")+type());
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

    std::string stdName = name().toStdString();
    auto c = network->getComponent(stdName);
    if (!c) {
        //FIXME during drag from XToolbar this will launch continuously, there is no xmas component yet
        // But ports are linked to their "connected" , isConnected asks for getPort()
        // getPort() asks for its owner and this owner asks for xmas_component and then we're here
        std::cerr << "xmas component for component " << stdName << " not found."
                  << "returning nullptr."<< std::endl;
        return nullptr;
    }

    return c;
}

QVariant model::Component::expression() {
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
        auto expr = func->getFunctionExpression(mp());
        if (expr != "") {
            return QString(expr.c_str()); // Only return xmas string, if useful
        } else {
            return m_expression;
        }
    }
    // In case of source return source specification.
    auto src = dynamic_cast<XMASSource *>(c);
    if (src) {
        auto expr = src->getSourceExpression(mp());
        if (expr != "") {
            return QString(expr.stl().c_str());  // Only return xmas string, if useful
        } else {
            return m_expression;
        }
    }
    // In case of switch return switch expression
    auto sw = dynamic_cast<XMASSwitch *>(c);
    if (sw) {
        auto expr = sw->getSwitchExpression(mp());
        if (expr != "") {
            return QString(expr.stl().c_str());  // Only return xmas string, if useful
        } else {
            return m_expression;
        }
    }
    // In case of join return join expression
    auto join = dynamic_cast<XMASJoin *>(c);
    if (join) {
        auto expr = join->getJoinExpression(mp());
        if (expr != "") {
            return QString(expr.stl().c_str());  // Only return xmas string, if useful
        } else {
            return m_expression;
        }
    }
    return m_expression;
}

void model::Component::expression(QVariant expression) {
    int errorPosition = -1;
    errorPosition = updateExpression(expression);
    emit expressionChanged(errorPosition);
}

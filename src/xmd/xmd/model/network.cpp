#include "network.h"

model::Network::Network(QQuickItem *parent)
    : QQuickItem(parent)
{
}

model::Network::~Network()
{

}
/** Connect method for qml */
bool model::Network::connect(XPort *outport, XPort *inport) {
    if (!outport) {
        QString errMsg = "[Network::connect()] outport is null.";
        emit writeLog(errMsg, Qt::red);
        qDebug() << errMsg;
        return false;
    }
    if (!inport) {
        QString errMsg = "[Network::connect()] inport is null.";
        emit writeLog(errMsg, Qt::red);
        qDebug() << errMsg;
        return false;
    }
    Output *xmas_outport = dynamic_cast<Output *>(outport->getPort());
    Input *xmas_inport = dynamic_cast<Input *>(inport->getPort());
    if (xmas_inport && xmas_outport) {
        ::connect(*xmas_outport, *xmas_inport);
        emit outport->connectedChanged();
        emit inport->connectedChanged();
        return true;
    }
    QString errMsg = "[Network::connect()] ";
    errMsg += (!xmas_inport ? (xmas_outport ? "xmas_inport and _outport are null."
                                           : "xmas_inport is null.")
                            : "xmas_outport is null.");
    emit writeLog(errMsg, Qt::red);
    qDebug() << errMsg;
    return false;
}
/** Disconnect method from output port for qml
 *
 *  Remark that we can disconnect from either input or output port.
 *  It does not matter for the result which we use.
 *  Port must be connected for disconnect to work.
 *
 *
 *
 */
bool model::Network::disconnect(XPort *outport, XPort *inport) {
    if (!outport) {
        QString errMsg = "[Network::disconnect()] port is null.";
        emit writeLog(errMsg);
        qDebug() << errMsg;
        return false;
    }
    qDebug() << "[model::Network::disconnect]";
    Output *xmas_outport = dynamic_cast<Output *>(outport->getPort());
    Input *xmas_inport = dynamic_cast<Input *>(outport->getPort());
    if (xmas_outport && xmas_outport->isConnected()) {
        ::disconnect(*xmas_outport);
        qDebug() << "xmas_output disconnect." << xmas_outport->isConnected();
        emit outport->connectedChanged();
        emit inport->connectedChanged();
        return true;
    } else if (xmas_inport && xmas_inport->isConnected()) {
        ::disconnect(*xmas_inport);
        qDebug() << "xmas_input disconnect." << xmas_inport->isConnected();
        emit outport->connectedChanged();
        emit inport->connectedChanged();
        return true;
    }
    QString errMsg = "[Network::disconnect()] inport or outport of connection null.";
    emit writeLog(errMsg);
    qDebug() << errMsg;
    return false;
}

QQmlListProperty<model::Component> model::Network::compList() {
    return QQmlListProperty<model::Component>(this, 0,      // The 0 = (void *)data
                                              &model::Network::append_compList,
                                              &model::Network::count_compList,
                                              &model::Network::at_compList,
                                              &model::Network::clear_compList
                                              );
}

/*****************************************************************************/
/*              Static methods: QQmlListProperty callbacks                   */
/*****************************************************************************/

void model::Network::append_compList(QQmlListProperty<model::Component> *property,
                     model::Component *comp) {
    Network *network = qobject_cast<Network *>(property->object);
    if (network) {
        comp->setParent(network);
        network->m_compList.append(comp);
    } else {
        std::string errMsg = "[Network.append_complist] network pointer zero ??";
        std::cout << errMsg << std::endl;
    }
}

int model::Network::count_compList(QQmlListProperty<model::Component> *property) {
    Network *network = qobject_cast<Network *>(property->object);
    if (network) {
        return network->m_compList.size();
    }
    std::string errMsg = "[Network.count_complist] network pointer zero ??";
    std::cout << errMsg << std::endl;
    return 0;
}

model::Component *model::Network::at_compList(QQmlListProperty<model::Component> *property,
                                       int index) {
    Network *network = qobject_cast<Network *>(property->object);
    if (network) {
        return network->m_compList.at(index);
    }
    std::string errMsg = "[Network.at_complist] network pointer zero ??";
    std::cout << errMsg << std::endl;
    return nullptr;
}

void model::Network::clear_compList(QQmlListProperty<model::Component> *property) {
    Network *network = qobject_cast<Network *>(property->object);
    if (network) {
        network->m_compList.clear();
    } else {
        std::string errMsg = "[Network.clear_complist] network pointer zero ??";
        std::cout << errMsg << std::endl;
    }
}

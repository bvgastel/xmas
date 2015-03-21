#include "network.h"

model::Network::Network(QQuickItem *parent)
    : QQuickItem(parent)
{
}

model::Network::~Network()
{

}

/**
 * Connect method for qml
 *
 * @brief model::Network::connect
 * @param outport
 * @param inport
 * @return xmas connection succeeded
 */
bool model::Network::connect(XPort *outport, XPort *inport) {
    // Check output
    if (!outport) {
        QString errMsg = "[Network::connect()] outport is null.";
        emit writeLog(errMsg, Qt::red);
        qDebug() << errMsg;
        return false;
    }
    // Check inport
    if (!inport) {
        QString errMsg = "[Network::connect()] inport is null.";
        emit writeLog(errMsg, Qt::red);
        qDebug() << errMsg;
        return false;
    }
    // obtain and check xmas inport and outport
    Output *xmas_outport = dynamic_cast<Output *>(outport->getPort());
    Input *xmas_inport = dynamic_cast<Input *>(inport->getPort());
    if (xmas_inport && xmas_outport) {
        ::connect(*xmas_outport, *xmas_inport);
        emit outport->connectedChanged();
        emit inport->connectedChanged();
        return true;

QString model::Network::toJson(QList<model::Component *> allComponents) {
    bitpowder::lib::String result;
    bitpowder::lib::MemoryPool mp;
    bitpowder::lib::JSONData globals;

    std::set<XMASComponent *> allComp;
    for (Component *comp : allComponents) {
        auto c = comp->getXMASComponent();
        allComp.insert(c);
    }
    result = ::Export(allComp,globals,mp);
    QString jsonString = QString(result.stl().c_str());
    return jsonString;
}

bool model::Network::disconnect(XPort *outport, XPort *inport) {
    // check outport
    if (!outport) {
        QString errMsg = "[Network::disconnect()] outport is null.";
        emit writeLog(errMsg);
        qDebug() << errMsg;
        return false;
    }
    //  check outport for being xmas outport to inport
    Output *xmas_outport = dynamic_cast<Output *>(outport->getPort());
    Input *xmas_inport = dynamic_cast<Input *>(inport->getPort());
    if (!xmas_outport->connectedTo(xmas_inport->m_owner)) {
        QString out_owner = xmas_outport->m_owner->getStdName().c_str();
        QString in_owner = xmas_inport->m_owner->getStdName().c_str();
        QString out_port_name = xmas_outport->getName();
        QString in_port_name = xmas_outport->getName();
        QString errMsg = "[Network::disconnect()] outport not connected to specified inport";
        errMsg += "\n[Network::disconnect()] output / input = "
                + out_owner+"."+out_port_name + ", "
                + in_owner + "." + in_port_name;
        emit writeLog(errMsg);
        qDebug() << errMsg;
        return false;
    }
    if (xmas_outport && xmas_outport->isConnected()) {
        ::disconnect(*xmas_outport);
        emit outport->connectedChanged();
        emit inport->connectedChanged();
        return true;
    } else if (xmas_inport && xmas_inport->isConnected()) {
        ::disconnect(*xmas_inport);
        emit outport->connectedChanged();
        emit inport->connectedChanged();
        return true;
    }
    QString errMsg = "[Network::disconnect()] inport or outport of connection null.";
    emit writeLog(errMsg);
    qDebug() << errMsg;
    return false;
}

void model::Network::childItemsChanged(){
    //NOTE : still working on , childItems in qml change and have the components but not here yet
    qDebug() << "childItemsChanged()";
}


//QQmlListProperty<model::Component> model::Network::compList() {
//    return QQmlListProperty<model::Component>(this, 0,      // The 0 = (void *)data
//                                              &model::Network::append_compList,
//                                              &model::Network::count_compList,
//                                              &model::Network::at_compList,
//                                              &model::Network::clear_compList
//                                              );
//}

bool model::Network::connect(XPort *outport, XPort *inport) {
    // Check output
    if (!outport) {
        QString errMsg = "[Network::connect()] outport is null.";
        emit writeLog(errMsg, Qt::red);
        qDebug() << errMsg;
        return false;
    }
    // Check inport
    if (!inport) {
        QString errMsg = "[Network::connect()] inport is null.";
        emit writeLog(errMsg, Qt::red);
        qDebug() << errMsg;
        return false;
    }
    // obtain and check xmas inport and outport
    Output *xmas_outport = dynamic_cast<Output *>(outport->getPort());
    Input *xmas_inport = dynamic_cast<Input *>(inport->getPort());
    if (xmas_inport && xmas_outport) {
        ::connect(*xmas_outport, *xmas_inport);
        emit outport->connectedChanged();
        emit inport->connectedChanged();
        return true;
    }
    // Trouble! Send error message
    QString errMsg = "[Network::connect()] ";
    errMsg += (!xmas_inport ? (xmas_outport ? "xmas_inport and _outport are null."
                                           : "xmas_inport is null.")
                            : "xmas_outport is null.");
    emit writeLog(errMsg, Qt::red);
    qDebug() << errMsg;
    return false;
}

/*****************************************************************************/
/*              Static methods: QQmlListProperty callbacks                   */
/*****************************************************************************/


///*****************************************************************************/
///*              Static methods: QQmlListProperty callbacks                   */
///*****************************************************************************/

//void model::Network::append_compList(QQmlListProperty<model::Component> *property,
//                     model::Component *comp) {
//    Network *network = qobject_cast<Network *>(property->object);
//    if (network) {
//        comp->setParent(network);
//        network->m_compList.append(comp);
//    } else {
//        std::string errMsg = "[Network.append_complist] network pointer zero ??";
//        std::cout << errMsg << std::endl;
//    }
//}

//int model::Network::count_compList(QQmlListProperty<model::Component> *property) {
//    Network *network = qobject_cast<Network *>(property->object);
//    if (network) {
//        return network->m_compList.size();
//    }
//    std::string errMsg = "[Network.count_complist] network pointer zero ??";
//    std::cout << errMsg << std::endl;
//    return 0;
//}

//model::Component *model::Network::at_compList(QQmlListProperty<model::Component> *property,
//                                       int index) {
//    Network *network = qobject_cast<Network *>(property->object);
//    if (network) {
//        return network->m_compList.at(index);
//    }
//    std::string errMsg = "[Network.at_complist] network pointer zero ??";
//    std::cout << errMsg << std::endl;
//    return nullptr;
//}

//void model::Network::clear_compList(QQmlListProperty<model::Component> *property) {
//    Network *network = qobject_cast<Network *>(property->object);
//    if (network) {
//        network->m_compList.clear();
//    } else {
//        std::string errMsg = "[Network.clear_complist] network pointer zero ??";
//        std::cout << errMsg << std::endl;
//    }
//}

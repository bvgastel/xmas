#include <QList>
#include <QUrl>

#include "util.h"
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


bool model::Network::openFile(QUrl url) {
    Q_UNUSED(url)
    //TODO open network via util instead of datacontrol
    return true;
}

bool model::Network::saveFile(QUrl url) {
    return Util::saveFile(url, toJson());
}

bool model::Network::closeFile() {
    //TODO destruct the model and reset everything
    return true;
}

bool model::Network::newFile() {
    return closeFile();
}

QString model::Network::toJson() {
    bitpowder::lib::String result;
    bitpowder::lib::MemoryPool mp;
    bitpowder::lib::JSONData globals = bitpowder::lib::JSONData::AllocateMap(mp);

    QList<model::Component *> allComponents = getAllComponents();

    std::set<XMASComponent *> allComp;
    for (Component *comp : allComponents) {
        auto c = comp->xmas_component();
        allComp.insert(c);
    }
//    result = ::Export(allComp,globals,mp);
//    QString jsonString = QString(result.stl().c_str());
//    return jsonString;
    return "";
}

bool model::Network::disconnect(XPort *outport, XPort *inport) {
    // check outport
    if (!outport) {
        QString errMsg = "[Network::disconnect()] outport is null.";
        emit writeLog(errMsg);
        qDebug() << errMsg;
        return false;
    }
    // check inport
    if (!inport) {
        QString errMsg = "[Network::disconnect()] inport is null.";
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

QQmlListProperty<model::Component> model::Network::components() {
    return QQmlListProperty<model::Component>(this, 0,      // The 0 = (void *)data
                                              &model::Network::append_components,
                                              &model::Network::count_components,
                                              &model::Network::at_components,
                                              &model::Network::clear_components
                                              );
}

bool model::Network::addComponent(model::Component *component) {
    auto xmas_comp = component->createXMASComponent(component->getType(), component->getName());
    component->xmas_component(xmas_comp);
    bool result;
    std::tie(std::ignore, result) = this->m_xmas_comp_list.insert(xmas_comp);
    return result;
}


/*****************************************************************************/
/*              Static methods: QQmlListProperty callbacks                   */
/*****************************************************************************/

void model::Network::append_components(QQmlListProperty<model::Component> *property,
                     model::Component *comp) {
    Network *network = qobject_cast<Network *>(property->object);
    if (network) {
        comp->setParent(network);
        network->m_components.append(comp);
    } else {
        std::string errMsg = "[Network.append_components] network pointer zero ??";
        std::cout << errMsg << std::endl;
    }
}

int model::Network::count_components(QQmlListProperty<model::Component> *property) {
    Network *network = qobject_cast<Network *>(property->object);
    if (network) {
        return network->m_components.size();
    }
    std::string errMsg = "[Network.count_components] network pointer zero ??";
    std::cout << errMsg << std::endl;
    return 0;
}

model::Component *model::Network::at_components(QQmlListProperty<model::Component> *property,
                                       int index) {
    Network *network = qobject_cast<Network *>(property->object);
    if (network) {
        return network->m_components.at(index);
    }
    std::string errMsg = "[Network.at_components] network pointer zero ??";
    std::cout << errMsg << std::endl;
    return nullptr;
}

void model::Network::clear_components(QQmlListProperty<model::Component> *property) {
    Network *network = qobject_cast<Network *>(property->object);
    if (network) {
        network->m_components.clear();
    } else {
        std::string errMsg = "[Network.clear_components] network pointer zero ??";
        std::cout << errMsg << std::endl;
    }
}

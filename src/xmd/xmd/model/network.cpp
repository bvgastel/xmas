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

/*
 * portError(port, errMsg) always return false to support error messaging.
 *
 */
bool model::Network::portError(XPort *port, QString errMsg) {
    errMsg += (port->getType() == model::XPort::OUTPORT ? " port = output_port! "
                                                           : " port = input_port! ");
    errMsg += port->getComponent()->getName()+"."+port->getName() + ". ";
    if (!port->getConnected()) {
        errMsg += "Port is not connected. ";
    }
    emit writeLog(errMsg, Qt::red);
    qDebug() << errMsg;
    return false;
}

bool model::Network::xmasError(Output *xmas_outport, Input *xmas_inport, QString errMsg) {
    QString out_owner = "null_output";
    QString out_port_name = "null";
    bool out_port_is_connected = false;
    QString in_owner = "null_input";
    QString in_port_name = "null";
    bool in_port_is_connected = false;

    if (xmas_outport) {
        if (xmas_outport->m_owner)
            out_owner = xmas_outport->m_owner->getStdName().c_str();
        out_port_name = xmas_outport->getName();
        out_port_is_connected = xmas_outport->isConnected();
    }
    if (xmas_inport) {
        if (xmas_inport->m_owner)
            in_owner = xmas_inport->m_owner->getStdName().c_str();
        in_port_name = xmas_outport->getName();
        in_port_is_connected = xmas_inport->isConnected();
    }

    errMsg += " " + out_owner+"."+out_port_name;
    errMsg += "->" + in_owner + "." + in_port_name + " ";
    if (in_port_is_connected && out_port_is_connected) {
        errMsg += "(both connected).";
    } else if (in_port_is_connected){
        errMsg += "(inport connected, outport not connected.";
    } else if (out_port_is_connected){
        errMsg += "(inport not connected).";
    } else {
        errMsg += "(both not connected).";
    }

    emit writeLog(errMsg);
    qDebug() << errMsg;
    return false;
}

bool model::Network::xmasConnectOk(Output *xmas_outport, Input *xmas_inport) {
    if (!xmas_inport || !xmas_outport) {
        return xmasError(xmas_outport, xmas_inport, "[Network::connect()] Connect failed: inport or outport null. ");
    }
    if (xmas_inport->isConnected() || xmas_outport->isConnected()) {
        return xmasError(xmas_outport, xmas_inport, "[Network::connect()] Connect failed: inport or outport already connected. ");
    }
    return true;
}

bool model::Network::connect(Output *xmas_outport, Input *xmas_inport) {
    if (xmasConnectOk(xmas_outport, xmas_inport)) {
        ::connect(*xmas_outport, *xmas_inport);
        return true;
    }
    return false;
}

bool model::Network::xmasDisconnectOk(Output *xmas_outport, Input *xmas_inport) {
    if (!xmas_outport->connectedTo(xmas_inport->m_owner)) {
        xmasError(xmas_outport, xmas_inport,
                  "[Network::disconnect(xmas)] outport not connected to specified inport");
        return false;
    }
    return true;
}

bool model::Network::disconnect(Output *xmas_outport, Input *xmas_inport) {
    bool success = xmasDisconnectOk(xmas_outport, xmas_inport);
    if (success) {
        ::disconnect(*xmas_outport);
        return true;
    }
    return false;
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

    // Check ports
    if (!outport) return portError(outport, "[Network::connect()] outport is null: ");
    if (!inport) return portError(inport, "[Network::connect()] inport is null.");

    // obtain and check xmas inport and outport
    Output *xmas_outport = dynamic_cast<Output *>(outport->getPort());
    Input *xmas_inport = dynamic_cast<Input *>(inport->getPort());
    bool success = connect(xmas_outport, xmas_inport);

    if (success) {
        emit outport->connectedChanged();
        emit inport->connectedChanged();
    }
    return success;
}

bool model::Network::disconnect(XPort *outport, XPort *inport) {

    // check outport
    if (!outport) return portError(outport, "[Network::disconnect()] outport is null: ");
    if (!inport) return portError(inport, "[Network::disconnect()] inport is null.");

    //  check outport for being xmas outport to inport
    Output *xmas_outport = dynamic_cast<Output *>(outport->getPort());
    Input *xmas_inport = dynamic_cast<Input *>(inport->getPort());
    bool success = disconnect(xmas_outport, xmas_inport);

    if (success) {
        emit outport->connectedChanged();
        emit inport->connectedChanged();
    }
    return success;
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
    //TODO destroy the model and reset everything
    return true;
}

bool model::Network::newFile() {
    return closeFile();
}

QString model::Network::toJson() {
    bitpowder::lib::String result;
    bitpowder::lib::MemoryPool mp;
    bitpowder::lib::JSONData globals = bitpowder::lib::JSONData::AllocateMap(mp);
    globals["PACKET_TYPE"] = bitpowder::lib::JSONData();        // to find out how to add packet type to json data.
    globals["VARS"] = bitpowder::lib::JSONData();               // to find out how to add vars to json data.

    QList<model::Component *> allComponents = getAllComponents();

    std::set<XMASComponent *> allComp;
    for (Component *comp : allComponents) {
        auto c = comp->xmas_component();
        allComp.insert(c);
    }
//    result = ::Export(allComp,globals,mp);
//    QString jsonString = QString(result.stl().c_str());
//    return jsonString;
    return "{"
           "\"VARS\": [],"
           "\"PACKET_TYPE\": {\"val \": 2},"
           "\"COMPOSITE_OBJECTS\": [],"
           "\"NETWORK\": ["
             "{\"id\":\"src0\",\"type\":\"source\",\"outs\":[{\"id\":\"q1\",\"in_port\":0}],\"pos\":{\"x\": 100,\"y\":100,\"orientation\":0,\"scale\":100},\"fields\":[{\"init_types\":\"value < 65384\"}]},"
             "{\"id\": \"q1\",\"type\": \"queue\",\"outs\": [{\"id\": \"q2\",\"in_port\": 0}],\"pos\": {\"x\": 210,\"y\": 210,\"orientation\": 90,\"scale\": 200},\"fields\": [{\"size\": 2}]},"
             "{\"id\": \"q2\",\"type\": \"queue\",\"outs\": [{\"id\": \"sink3\",\"in_port\": 0}],\"pos\": {\"x\": 310,\"y\": 310,\"orientation\": 0,\"scale\": 100},\"fields\": [{\"size\": 5}]},"
             "{\"id\": \"sink3\",\"type\": \"sink\",\"pos\": {\"x\": 410,\"y\": 410,\"orientation\": 0,\"scale\": 100}}"
             "]"
         "}";
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

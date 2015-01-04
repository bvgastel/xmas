#ifndef DESIGNERCONTROLLER_H
#define DESIGNERCONTROLLER_H

#include "positioncomponentextension.h"
#include "network.h"
#include "checker/xmas.h"

class DesignerController
{
public:
    DesignerController(Network& network);
    ~DesignerController();

    template<class T>
    XMASComponent* addComponent(const std::string name, int x, int y, Orientation orientation);

    void addChannel(Output& output, Input& input);

private:
    Network& m_network;
};

template<class T>
XMASComponent* DesignerController::addComponent(const std::string name, int x, int y, Orientation orientation)
{
    std::unique_ptr<T> c {new T{name}};
    std::unique_ptr<PositionComponentExtension> pce {new PositionComponentExtension{x, y, orientation}};

    c->addExtension(pce.release());

    return m_network.addComponent(c.release());
}

#endif // DESIGNERCONTROLLER_H

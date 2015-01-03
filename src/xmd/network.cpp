#include "network.h"
#include "checker/xmas.h"
#include <iterator>

Network::Network()
{

}

Network::~Network()
{

}

XMASComponent* Network::addComponent(XMASComponent *component)
{
    std::unique_ptr<XMASComponent> c {component};                           // take ownership of the component
    
    const std::string name = static_cast<std::string>(c->getName());

    auto iter = m_components.emplace(std::make_pair(name, std::move(c)));	// pass ownership to m_components
    if (!iter.second)
        throw NetworkException("Component already exists");

    emit componentAdded(component);
    
    return component;
}

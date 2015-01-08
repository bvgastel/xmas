/*********************************************************************
 *
 * Copyright (C) Guus Bonnema, 2014
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
#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory> // for shared_ptr, make_shared
#include <utility> // for pair

#include <QString>
#include <QHash>
#include <QVector>
#include <QDataStream>

#include "xmas_global.h"

// namespace xmas {

class Component;


/**
 * @brief The Component class
 *
 * The component on an Noc. It contains ports (both
 * in ports and out ports.
 *
 * This class contains the nested definition and declaration of Port.
 * This reflects the tight coupling : outside the Component these objects
 * would not exist.
 *
 *
 */
class XMASSHARED_EXPORT Component
{

    friend std::ostream &operator<< (std::ostream &os, const Component &comp);

    friend QDataStream &operator<< (QDataStream &os, const Component &comp);
    friend QDataStream &operator<< (QDataStream &os, std::shared_ptr<const Component> comp);
    friend QDataStream &operator>> (QDataStream &ds, Component &comp);
    friend QDataStream &operator>> (QDataStream &ds, std::shared_ptr<Component> &comp);

    friend bool operator== (const Component, const Component);
    friend bool operator!= (const Component, const Component);

public:
    typedef QString Function;
    typedef std::pair<QString, QString> PortNamePropPair;
    typedef QVector<PortNamePropPair> PortList;

    /* Nested classes */
    class Port;

    /**
     * @brief Component Default constructor
     *
     * The default constructor is meant to be used only for creating
     * a component from an input stream or a QDataStream.
     *
     */
    Component() {

    }

    Component(QString name,
              PortList inport_list,
              PortList outport_list,
              QString function="");
    virtual ~Component()
    {

    }

    inline QString name() const { return m_name; }
    inline void name(const QString name) { m_name = name; }

    inline Function function() const { return m_function; }

    int in_size() const;
    int out_size() const;

    // Need inport and outport iterators
    PortList inList() const;
    PortList outList() const;

    Function rdy(QString portName) const;

    bool portExists(QString name) const;
    bool inportExists(QString name) const;
    bool outportExists(QString name) const;

private:

    /**
     * @brief portList a utility function for inList() and outList()
     *
     * @param portmap the input port map or the output port map
     * @return the PortList
     */
    PortList portList(QHash<QString, std::shared_ptr<Port>> portmap) const;

    /**
     * @brief m_name the name of the component
     */
    QString m_name;
    /**
     * @brief m_inport_map A map of input ports by name
     */
    QHash<QString, std::shared_ptr<Port>> m_inport_map;
    /**
     * @brief m_outport_map A map of output ports by name
     */
    QHash<QString, std::shared_ptr<Port>> m_outport_map;

    /**
     * @brief m_function a function body
     *
     * The function body is empty if not applicable.
     * It is unstructured information for the verification
     * tools and the reader of the component.
     *
     */
    Function m_function;


public:
    /**
     * @brief The Port class
     *
     * The class to represent both in ports and out ports in a component.
     * This class is nested inside Component as it is inherently
     * connected to Component. Its role (input or output) depends
     * on the component. The name identifies the port within the
     * component.
     *
     */
    class Port
    {
        friend QDataStream &operator<< (QDataStream &ds, const Port &comp);
        friend QDataStream &operator>> (QDataStream &ds, Port &port);
        friend QDataStream &operator<< (QDataStream &ds, std::shared_ptr<const Component::Port> port);
        friend QDataStream &operator>> (QDataStream &ds, std::shared_ptr<Component::Port> &port);
        friend bool operator==(const Port, const Port);

    public:

        /**
         * @brief Port
         *
         * The default constructor of Port is meant to be used only
         * by operator overrides for constructing a Port instance
         * from a default (empty) Port.
         *
         * Normally one should never use this constructor outside serialization.
         *
         */
        Port() {}
        Port(QString name, QString rdy);
        ~Port();

        inline QString name() const { return m_name; }
        inline QString rdy() const {return m_rdy; }

    private:
        QString m_name;
        Function m_rdy;

    };

};

std::ostream &operator<< (std::ostream &os, const Component &comp);
std::ostream &operator<< (std::ostream &os, const Component::Port &port);

QDataStream &operator<< (QDataStream &ds, const Component &comp);
QDataStream &operator<< (QDataStream &ds, const Component::Port &comp);
QDataStream &operator<< (QDataStream &ds, std::shared_ptr<const Component::Port> port);

QDataStream &operator>> (QDataStream &ds, Component &comp);
QDataStream &operator>> (QDataStream &ds, std::shared_ptr<Component> &comp);

QDataStream &operator>> (QDataStream &ds, Component::Port &port);
QDataStream &operator>> (QDataStream &ds, std::shared_ptr<Component::Port> &port);

bool checkPortEq(const QHash<QString, std::shared_ptr<Component::Port>> &left, const QHash<QString, std::shared_ptr<Component::Port>> &right);

bool operator== (const Component, const Component);
inline bool operator!= (const Component lcomp, const Component rcomp) { return !(lcomp == rcomp); }

bool operator==(const Component::Port, const Component::Port);
inline bool operator!=(const Component::Port l_port, const Component::Port r_port) {return !(l_port == r_port); }

//} // end of namespace xmas

#endif // COMPONENT_H

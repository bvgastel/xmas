/*********************************************************************
  *
  * Copyright (C) Bonnema, 2015
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


#ifndef FUNCTION_H
#define FUNCTION_H

#include "component.h"

namespace model {

class Function : public Component
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(QVariantList specList READ specList WRITE specList NOTIFY specListChanged)

public:
    Function(QQuickItem *parent=0);
    ~Function();

    virtual void classBegin();
    virtual void componentComplete();

signals:
    void specListChanged();

public slots:

public:
    QVariantList specList() {
        return m_specList;
    }

    void specList(QVariantList specList) {
        m_specList = specList;
    }

private:
    QVariantList m_specList;
};

} // namespace model

#endif // FUNCTION_H

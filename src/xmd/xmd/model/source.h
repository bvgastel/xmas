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

#ifndef SOURCE_H
#define SOURCE_H

#include "component.h"

namespace model {

class Source : public Component
{
    Q_OBJECT
    Q_PROPERTY(QString specification READ specification WRITE specification NOTIFY specificationChanged)

public:
    Source(QQuickItem *parent = 0);
    ~Source();

signals:
    void specificationChanged();

public slots:

public:
    QString specification() {
        return m_specification;
    }
    void specification(QString specification) {
        m_specification = specification;
    }

private:
    QString m_specification;
};

} // namespace model

#endif // SOURCE_H

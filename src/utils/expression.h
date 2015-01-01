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

#ifndef EXPRESSION_H
#define EXPRESSION_H


class Expression
{
public:
    Expression();
    ~Expression();
};


// Answer from http://stackoverflow.com/questions/18691859/building-a-graph-from-boolean-expression-in-c
//
// You could use expression templates to generate the syntax tree of the expression:

class Boolean
{
public:
    Boolean(bool b) : _b( b ) {}

private:
    bool _b;
};

template<typename LHS , typename RHS>
struct parallel
{
    LHS& left;
    RHS& right;

    parallel(const LHS& _left , const RHS& _right) : left( _left ) , right( _right ) {}
};

template<typename LHS , typename RHS>
struct serie
{
    LHS& first;
    RHS& second;

    serie(const LHS& _left , const RHS& _right) : first( _left ) , second( _right ) {}
};


/* Boolean vs Boolean parallel expression */
parallel<Boolean,Boolean> operator+(const Boolean& lhs , const Boolean& rhs)
{
    return parallel<Boolean,Boolean>( lhs , rhs );
}

/* Boolean vs Boolean serie expression */
serie<Boolean,Boolean> operator*(const Boolean& lhs , const Boolean& rhs)
{
    return serie<Boolean,Boolean>( lhs , rhs );
}


/* Boolean vs internal node parallel expressions */

template<typename LHS , typename RHS>
parallel<parallel<LHS,RHS>,Boolean> operator+(const parallel<LHS,RHS>& lhs , const Boolean& rhs)
{
    return parallel<parallel<LHS,RHS>,Boolean>( lhs , rhs );
}

template<typename LHS , typename RHS>
parallel<Boolean,parallel<LHS,RHS>> operator+(const Boolean& lhs , const parallel<LHS,RHS>& rhs)
{
    return parallel<Boolean,parallel<LHS,RHS>>( lhs , rhs );
}

template<typename LHS , typename RHS>
parallel<serie<LHS,RHS>,Boolean> operator+(const serie<LHS,RHS>& lhs , const Boolean& rhs)
{
    return parallel<serie<LHS,RHS>,Boolean>( lhs , rhs );
}

template<typename LHS , typename RHS>
parallel<Boolean,serie<LHS,RHS>> operator+(const Boolean& lhs , const serie<LHS,RHS>& rhs)
{
    return parallel<Boolean,serie<LHS,RHS>>( lhs , rhs );
}


/* Boolean vs internal node serie expresssions */

template<typename LHS , typename RHS>
serie<parallel<LHS,RHS>,Boolean> operator*(const parallel<LHS,RHS>& lhs , const Boolean& rhs)
{
    return serie<parallel<LHS,RHS>,Boolean>( lhs , rhs );
}

template<typename LHS , typename RHS>
serie<Boolean,parallel<LHS,RHS>> operator*(const Boolean& lhs , const parallel<LHS,RHS>& rhs)
{
    return serie<Boolean,parallel<LHS,RHS>>( lhs , rhs );
}

template<typename LHS , typename RHS>
serie<serie<LHS,RHS>,Boolean> operator*(const serie<LHS,RHS>& lhs , const Boolean& rhs)
{
    return serie<serie<LHS,RHS>,Boolean>( lhs , rhs );
}

template<typename LHS , typename RHS>
serie<Boolean,serie<LHS,RHS>> operator*(const Boolean& lhs , const serie<LHS,RHS>& rhs)
{
    return serie<Boolean,serie<LHS,RHS>>( lhs , rhs );
}


/* internal node vs internal node parallel expressions */

template<typename LHS1 , typename RHS1 , typename LHS2 , typename RHS2>
parallel<parallel<LHS1,RHS1>,parallel<LHS2,RHS2>> operator+(const parallel<LHS1,RHS1>& lhs , const parallel<LHS2,RHS2>& rhs)
{
    return parallel<parallel<LHS1,RHS1>,parallel<LHS2,RHS2>>( lhs , rhs );
}

template<typename LHS1 , typename RHS1 , typename LHS2 , typename RHS2>
parallel<serie<LHS1,RHS1>,serie<LHS2,RHS2>> operator+(const serie<LHS1,RHS1>& lhs , const serie<LHS2,RHS2>& rhs)
{
    return parallel<serie<LHS1,RHS1>,serie<LHS2,RHS2>>( lhs , rhs );
}

template<typename LHS1 , typename RHS1 , typename LHS2 , typename RHS2>
parallel<parallel<LHS1,RHS1>,serie<LHS2,RHS2>> operator+(const parallel<LHS1,RHS1>& lhs , const serie<LHS2,RHS2>& rhs)
{
    return parallel<parallel<LHS1,RHS1>,serie<LHS2,RHS2>>( lhs , rhs );
}

template<typename LHS1 , typename RHS1 , typename LHS2 , typename RHS2>
parallel<serie<LHS1,RHS1>,parallel<LHS2,RHS2>> operator+(const serie<LHS1,RHS1>& lhs , const parallel<LHS2,RHS2>& rhs)
{
    return parallel<serie<LHS1,RHS1>,parallel<LHS2,RHS2>>( lhs , rhs );
}


/* internal node vs internal node serie expressions */

template<typename LHS1 , typename RHS1 , typename LHS2 , typename RHS2>
serie<parallel<LHS1,RHS1>,parallel<LHS2,RHS2>> operator*(const parallel<LHS1,RHS1>& lhs , const parallel<LHS2,RHS2>& rhs)
{
    return serie<parallel<LHS1,RHS1>,parallel<LHS2,RHS2>>( lhs , rhs );
}

template<typename LHS1 , typename RHS1 , typename LHS2 , typename RHS2>
serie<serie<LHS1,RHS1>,serie<LHS2,RHS2>> operator*(const serie<LHS1,RHS1>& lhs , const serie<LHS2,RHS2>& rhs)
{
    return serie<serie<LHS1,RHS1>,serie<LHS2,RHS2>>( lhs , rhs );
}

template<typename LHS1 , typename RHS1 , typename LHS2 , typename RHS2>
serie<parallel<LHS1,RHS1>,serie<LHS2,RHS2>> operator*(const parallel<LHS1,RHS1>& lhs , const serie<LHS2,RHS2>& rhs)
{
    return serie<parallel<LHS1,RHS1>,serie<LHS2,RHS2>>( lhs , rhs );
}

template<typename LHS1 , typename RHS1 , typename LHS2 , typename RHS2>
serie<serie<LHS1,RHS1>,parallel<LHS2,RHS2>> operator*(const serie<LHS1,RHS1>& lhs , const parallel<LHS2,RHS2>& rhs)
{
    return serie<serie<LHS1,RHS1>,parallel<LHS2,RHS2>>( lhs , rhs );
}

#endif // EXPRESSION_H

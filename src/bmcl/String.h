/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <string>
#include <vector>
#include <sstream>

#include "bmcl/Config.h"

#if defined(BMCL_HAVE_QT)
#include <QString>
#endif

namespace bmcl
{

template<class T>
std::string toString(const T & t);

template<class T>
inline std::string toString(const std::vector<T> & t)
{
    std::ostringstream s("[");
    bool isFirst = true;
    for (const auto & el : t)
    {
        if (isFirst)
            isFirst = false;
        else
            s << ", ";
        s << toString(el);
    }
    s << "]";
    return s.str();
}

template<>
inline std::string toString(const std::string & t)
{
    return t;
}

inline std::string toString(const char * t)
{
    return std::string(t);
}

template<class T>
inline std::string toString(const T & t)
{
    return std::to_string(t);
}

#if defined(BMCL_HAVE_QT)
template<>
inline std::string toString(const QString & t)
{
    return t.toStdString();
}
#endif

BMCL_EXPORT std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);

BMCL_EXPORT std::vector<std::string> split(const std::string &s, char delim);

}

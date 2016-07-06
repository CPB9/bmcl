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
inline std::string toString(const T & t) { return std::to_string(t); }

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
inline std::string toString(const std::string & t) { return t; }

inline std::string toString(const char * t) { return std::string(t); }

#if defined(BMCL_HAVE_QT)
template<>
inline std::string toString(const QString & t) { return t.toStdString(); }
#endif

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);

std::vector<std::string> split(const std::string &s, char delim);

}

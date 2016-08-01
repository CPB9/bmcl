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
std::string toString(const std::vector<T> & t);

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

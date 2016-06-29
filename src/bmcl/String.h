#pragma once

#include <string>
#include <vector>

#include "bmcl/Config.h"

#if defined(BMCL_HAVE_QT)
#include <QString>
#endif

namespace bmcl
{

template<class T>
inline std::string toString(const T & t) BMCL_NOEXCEPT { return std::to_string(t); }

template<>
inline std::string toString(const std::string & t) BMCL_NOEXCEPT { return t; }

#if defined(BMCL_HAVE_QT)
template<>
inline std::string toString(const QString & t) BMCL_NOEXCEPT { return t.toStdString(); }
#endif

template<class T>
inline const char * toCString(const T & t) BMCL_NOEXCEPT { return toString(t).c_str(); }

BMCL_EXPORT inline const char * toCString(const char * t) BMCL_NOEXCEPT { return t; }

BMCL_EXPORT std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);

BMCL_EXPORT std::vector<std::string> split(const std::string &s, char delim);

}

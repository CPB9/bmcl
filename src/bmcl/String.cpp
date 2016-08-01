#include <sstream>

#include "bmcl/String.h"

namespace bmcl
{

template<class T>
std::string toString(const std::vector<T> & t)
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

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems)
{
    std::istringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim))
        elems.push_back(item);
    return elems;
}

std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

}


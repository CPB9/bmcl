#include <sstream>

#include "bmcl/String.h"

namespace bmcl
{

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


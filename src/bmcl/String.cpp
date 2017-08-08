/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <sstream>

#include "bmcl/String.h"
#include "bmcl/Assert.h"

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

static char nibbleToHexCharLower(uint8_t nibble)
{
    switch(nibble) {
    case 0:
        return '0';
    case 1:
        return '1';
    case 2:
        return '2';
    case 3:
        return '3';
    case 4:
        return '4';
    case 5:
        return '5';
    case 6:
        return '6';
    case 7:
        return '7';
    case 8:
        return '8';
    case 9:
        return '9';
    case 10:
        return 'a';
    case 11:
        return 'b';
    case 12:
        return 'c';
    case 13:
        return 'd';
    case 14:
        return 'e';
    case 15:
        return 'f';
    }
    BMCL_ASSERT(false);
}

static char nibbleToHexCharUpper(uint8_t nibble)
{
    switch(nibble) {
    case 0:
        return '0';
    case 1:
        return '1';
    case 2:
        return '2';
    case 3:
        return '3';
    case 4:
        return '4';
    case 5:
        return '5';
    case 6:
        return '6';
    case 7:
        return '7';
    case 8:
        return '8';
    case 9:
        return '9';
    case 10:
        return 'A';
    case 11:
        return 'B';
    case 12:
        return 'C';
    case 13:
        return 'D';
    case 14:
        return 'E';
    case 15:
        return 'F';
    }
    BMCL_ASSERT(false);
}

template <char (*conv)(uint8_t)>
inline std::string bytesToHexStringBase(const uint8_t* data, std::size_t size)
{
    std::string str(size * 2, ' ');
    for (std::size_t i = 0; i < size; i++) {
        uint8_t byte = data[i];
        str[i*2] = conv((byte & 0xf0) >> 4);
        str[i*2 + 1] = conv(byte & 0x0f);
    }
    return str;

}

std::string bytesToHexStringLower(const uint8_t* data, std::size_t size)
{
    return bytesToHexStringBase<nibbleToHexCharLower>(data, size);
}

std::string bytesToHexStringUpper(const uint8_t* data, std::size_t size)
{
    return bytesToHexStringBase<nibbleToHexCharUpper>(data, size);
}
}


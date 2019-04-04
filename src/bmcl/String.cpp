/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <sstream>

#include "bmcl/String.h"

#include <cassert>

namespace bmcl
{

std::vector<std::string>& split(const std::string& s, char delim, std::vector<std::string>& elems)
{
    std::istringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

static inline char nibbleToHexCharLower(uint8_t nibble)
{
    assert(nibble < 16);
    const char* chars = "0123456789abcdef";
    return chars[nibble];
}

static inline char nibbleToHexCharUpper(uint8_t nibble)
{
    assert(nibble < 16);
    const char* chars = "0123456789ABCDEF";
    return chars[nibble];
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


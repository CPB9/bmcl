/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "bmcl/CString.h"

#include <string.h>
#include <cstring>
#include <cstdlib>

namespace bmcl {

void CString::destruct()
{
    if (_str) {
        std::free(_str);
    }
}

char* CString::duplicateString(const char* other)
{
    if (other) {
#ifdef _WIN32
    return _strdup(other);
#else
    return strdup(other);
#endif
    }
    return nullptr;
}

std::size_t CString::size() const
{
    return std::strlen(_str);
}

bool CString::operator==(const CString& other) const
{
    return std::strcmp(_str, other._str) == 0;
}

bool CString::operator!=(const CString& other) const
{
    return std::strcmp(_str, other._str) != 0;
}

bool CString::operator<(const CString& other) const
{
    return std::strcmp(_str, other._str) < 0;
}

bool CString::operator<=(const CString& other) const
{
    return std::strcmp(_str, other._str) <= 0;
}

bool CString::operator>(const CString& other) const
{
    return std::strcmp(_str, other._str) > 0;
}

bool CString::operator>=(const CString& other) const
{
    return std::strcmp(_str, other._str) >= 0;
}
}

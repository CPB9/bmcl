/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/Config.h"

#include <cstddef>

namespace bmcl {

class CString {
public:
    CString();
    explicit CString(const char* str);
    CString(const CString& other);
    CString(CString&& other);
    ~CString();

    CString& operator=(const CString& other);
    CString& operator=(CString&& other);

    static CString fromOwned(char* str);

    const char* data() const;
    char* data();

    bool isNull() const;
    std::size_t size() const;

    bool operator==(const CString& other) const;
    bool operator!=(const CString& other) const;
    bool operator<(const CString& other) const;
    bool operator<=(const CString& other) const;
    bool operator>(const CString& other) const;
    bool operator>=(const CString& other) const;

private:
    void destruct();
    static char* duplicateString(const char* other);

    char* _str;
};

inline CString::CString()
    : _str(0)
{
}

inline CString::CString(const char* str)
    : _str(duplicateString(str))
{
}

inline CString::~CString()
{
    destruct();
}

inline CString::CString(const CString& other)
    : _str(duplicateString(other._str))
{
}

inline CString::CString(CString&& other)
    : _str(other._str)
{
    other._str = nullptr;
}

inline CString& CString::operator=(const CString& other)
{
    destruct();
    _str = duplicateString(other._str);
    return *this;
}

inline CString& CString::operator=(CString&& other)
{
    destruct();
    _str = other._str;
    other._str = nullptr;
    return *this;
}

inline CString CString::fromOwned(char* str)
{
    CString cstring;
    cstring._str = str;
    return cstring;
}

inline char* CString::data()
{
    return _str;
}

inline const char* CString::data() const
{
    return _str;
}

inline bool CString::isNull() const
{
    return _str == nullptr;
}
}

/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/Config.h"
#include "bmcl/ArrayView.h"

#include <algorithm>
#include <cstddef>
#include <cstring>
#include <string>

namespace bmcl {

template <typename T>
class Option;

class StringView : public ArrayViewBase<char, StringView> {
public:
    StringView(const char* cstr);
    StringView(const char* data, std::size_t size);
    StringView(const char* start, const char* end);
    StringView(const std::string& str);

    std::string toStdString() const;
    std::string toUpper() const;
    std::string toLower() const;

    Bytes asBytes() const;

    bool startsWith(StringView prefix) const;
    bool endsWith(StringView postfix) const;

    bool equals(StringView other) const;
    bool equals(const std::string& other) const;
    bool equals(const char* cstr) const;

    Option<std::size_t> findFirstOf(char c, std::size_t from = 0) const;
    Option<std::size_t> findFirstOf(StringView chars, std::size_t from = 0) const;
    Option<std::size_t> findFirstNotOf(char c, std::size_t from = 0) const;
    Option<std::size_t> findFirstNotOf(StringView chars, std::size_t from = 0) const;

    Option<std::size_t> findLastOf(char c, std::size_t offset = 0) const;
    Option<std::size_t> findLastOf(StringView chars, std::size_t offset = 0) const;
    Option<std::size_t> findLastNotOf(char c, std::size_t offset = 0) const;
    Option<std::size_t> findLastNotOf(StringView chars, std::size_t offset = 0) const;

    StringView ltrim(char c) const;
    StringView rtrim(char c) const;
    StringView trim(char c) const;

    StringView ltrim(StringView chars = " \t\n\v\f\r") const;
    StringView rtrim(StringView chars = " \t\n\v\f\r") const;
    StringView trim(StringView chars = " \t\n\v\f\r") const;

    bool operator==(StringView other) const;

private:
    template <typename C>
    std::string map(C&& convert) const;
    Option<std::size_t> iteratorToIndex(iterator) const;
    Option<std::size_t> iteratorToIndex(reverse_iterator) const;
};

inline StringView::StringView(const char* cstr)
    : ArrayViewBase<char, StringView>(cstr, cstr ? std::strlen(cstr) : 0)
{
}

inline StringView::StringView(const char* start, const char* end)
    : ArrayViewBase<char, bmcl::StringView>(start, end)
{
}

inline StringView::StringView(const std::string& str)
    : ArrayViewBase<char, StringView>(str.c_str(), str.size())
{
}

inline StringView::StringView(const char* data, std::size_t size)
    : ArrayViewBase<char, StringView>(data, size)
{
}

inline Bytes StringView::asBytes() const
{
    return Bytes((const uint8_t*)data(), size());
}

inline bool StringView::startsWith(StringView prefix) const
{
    return (size() >= prefix.size()) && (std::memcmp(data(), prefix.data(), prefix.size()) == 0);
}

inline bool StringView::endsWith(StringView prefix) const
{
    return (size() >= prefix.size()) && (std::memcmp(end() - prefix.size(), prefix.data(), prefix.size()) == 0);
}

inline bool StringView::equals(StringView other) const
{
    return (size() == other.size()) && (std::memcmp(data(), other.data(), size()) == 0);
}

inline bool StringView::equals(const std::string& other) const
{
    return (size() == other.size()) && (std::memcmp(data(), other.data(), size()) == 0);
}

inline bool StringView::equals(const char* cstr) const
{
    std::size_t otherSize = std::strlen(cstr);
    return (size() == otherSize) && (std::memcmp(data(), cstr, size()) == 0);
}

inline std::string StringView::toStdString() const
{
    return std::string(data(), size());
}

inline bool StringView::operator==(StringView other) const
{
    return equals(other);
}

inline bool operator==(const std::string& lhs, StringView rhs)
{
    return rhs.equals(lhs);
}

inline bool operator==(StringView lhs, const std::string& rhs)
{
    return lhs.equals(rhs);
}

inline bool operator==(const char* lhs, StringView rhs)
{
    return rhs.equals(lhs);
}

inline bool operator==(StringView lhs, const char* rhs)
{
    return lhs.equals(rhs);
}
}

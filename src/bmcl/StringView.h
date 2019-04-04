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
#include "bmcl/Fwd.h"

#include <cassert>
#include <cstddef>
#include <cstring>
#include <string>

namespace bmcl {

class BMCL_EXPORT StringView : public ArrayViewBase<char, StringView> {
public:
    constexpr inline StringView();
    constexpr inline StringView(std::nullptr_t);

    BMCL_CONSTEXPR_CPP17 inline StringView(const char* cstr);
    constexpr inline StringView(const char* data, std::size_t size);
    constexpr inline StringView(const char* start, const char* end);
    inline StringView(const std::string& str);
    inline StringView(ArrayView<char> view);
    template <std::size_t S>
    inline StringView(FixedArrayView<char, S> view);

    inline std::string toStdString() const;
    std::string toUpper() const;
    std::string toLower() const;

    inline Bytes asBytes() const;

    inline bool startsWith(StringView prefix) const;
    inline bool endsWith(StringView postfix) const;

    OptionSize findFirstOf(char c, std::size_t from = 0) const;
    OptionSize findFirstOf(StringView chars, std::size_t from = 0) const;
    OptionSize findFirstNotOf(char c, std::size_t from = 0) const;
    OptionSize findFirstNotOf(StringView chars, std::size_t from = 0) const;

    OptionSize findLastOf(char c, std::size_t offset = 0) const;
    OptionSize findLastOf(StringView chars, std::size_t offset = 0) const;
    OptionSize findLastNotOf(char c, std::size_t offset = 0) const;
    OptionSize findLastNotOf(StringView chars, std::size_t offset = 0) const;

    StringView ltrim(char c) const;
    StringView rtrim(char c) const;
    StringView trim(char c) const;

    StringView ltrim(StringView chars = " \t\n\v\f\r") const;
    StringView rtrim(StringView chars = " \t\n\v\f\r") const;
    StringView trim(StringView chars = " \t\n\v\f\r") const;

private:
    template <typename C>
    std::string map(C&& convert) const;
    OptionSize iteratorToIndex(iterator) const;
    OptionSize iteratorToIndex(reverse_iterator) const;
};

constexpr inline StringView::StringView()
    : ArrayViewBase<char, StringView>("", std::size_t(0))
{
}

constexpr inline StringView::StringView(std::nullptr_t)
    : StringView()
{
}

BMCL_CONSTEXPR_CPP17 inline StringView::StringView(const char* cstr)
    : ArrayViewBase<char, StringView>(cstr, std::char_traits<char>::length(cstr))
{
}

constexpr inline StringView::StringView(const char* start, const char* end)
    : ArrayViewBase<char, StringView>(start, end)
{
}

inline StringView::StringView(const std::string& str)
    : ArrayViewBase<char, StringView>(str.c_str(), str.size())
{
}

inline StringView::StringView(ArrayView<char> view)
    : ArrayViewBase<char, StringView>(view.data(), view.size())
{
}

constexpr inline StringView::StringView(const char* data, std::size_t size)
    : ArrayViewBase<char, StringView>(data, size)
{
}

template <std::size_t S>
inline StringView::StringView(FixedArrayView<char, S> view)
    : ArrayViewBase<char, StringView>(view.data(), view.size())
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

inline std::string StringView::toStdString() const
{
    return std::string(data(), size());
}

template <typename T>
inline bool operator==(const T& lhs, StringView rhs)
{
    return StringView(lhs).operator==(rhs);
}

template <typename T>
inline bool operator!=(const T& lhs, StringView rhs)
{
    return StringView(lhs).operator!=(rhs);
}
}

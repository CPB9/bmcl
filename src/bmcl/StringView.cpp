/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "bmcl/Config.h"
#include "bmcl/StringView.h"
#include "bmcl/Option.h"

#include <bitset>
#include <climits>

namespace bmcl {

static char asciiToUpper(char c)
{
    if ( c >= 'a'&& c <= 'z' ) {
        return c + ('Z' - 'z');
    }
    return c;
}

static inline char asciiToLower(char c)
{
    if ( c >= 'A'&& c <= 'Z' ) {
        return c - ('Z' - 'z');
    }
    return c;
}

template <typename C>
std::string StringView::map(C&& convert) const
{
    std::string result;
    result.resize(size());
    std::transform(data(), data() + size(), result.begin(), convert);
    return result;
}

std::string StringView::toLower() const
{
    return map(asciiToLower);
}

std::string StringView::toUpper() const
{
    return map(asciiToUpper);
}

inline Option<std::size_t> StringView::iteratorToIndex(const char* it) const
{
    if (it == end()) {
        return None;
    }
    return it - begin();
}

Option<std::size_t> StringView::findFirstOf(char c, std::size_t from) const
{
    BMCL_ASSERT(from <= size());
    return iteratorToIndex(std::find(begin() + from, end(), c));
}

Option<std::size_t> StringView::findFirstOf(StringView chars, std::size_t from) const
{
    BMCL_ASSERT(from <= size());
    std::bitset<1 << CHAR_BIT> bits;
    for (char c : chars) {
        bits.set(c);
    }
    return iteratorToIndex(std::find_if(begin() + from, end(), [&bits](char c){
        return bits.test(c);
    }));
}

Option<std::size_t> StringView::findFirstNotOf(char c, std::size_t from) const
{
    BMCL_ASSERT(from <= size());
    return iteratorToIndex(std::find_if(begin() + from, end(), [c](char s){
        return s != c;
    }));
}

Option<std::size_t> StringView::findFirstNotOf(StringView chars, std::size_t from) const
{
    BMCL_ASSERT(from <= size());
    std::bitset<1 << CHAR_BIT> bits;
    for (char c : chars) {
        bits.set(c);
    }
    return iteratorToIndex(std::find_if(begin() + from, end(), [&bits](char c){
        return !bits.test(c);
    }));
}

StringView StringView::ltrim(char c) const
{
    const char* it = begin();
    while (it != end() && *it == c) {
        it++;
    }
    return StringView(it, end());
}

StringView StringView::rtrim(char c) const
{
    const char* it = end();
    while (it != begin() && *(it - 1) == c) {
        it--;
    }
    return StringView(begin(), it);
}

StringView StringView::trim(char c) const
{
    return ltrim(c).rtrim(c);
}
}

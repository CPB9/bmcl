/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/Config.h"
#include "bmcl/Fwd.h"

#include <iterator>
#include <type_traits>
#include <cstddef>
#include <cassert>
#include <array>
#include <vector>
#include <initializer_list>

namespace bmcl {

template<class B, class T>
using enableIfBase = typename std::enable_if<std::is_base_of<B, T>::value, B>::type;

template <typename T, typename B>
class ArrayViewBase {
public:
    typedef T value_type;
    typedef std::size_t size_type;
    typedef const T* iterator;
    typedef const T* const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;

    constexpr ArrayViewBase();
    template <std::size_t N>
    ArrayViewBase(const std::array<T, N>& lst);
    template <std::size_t N>
    ArrayViewBase(FixedArrayView<T, N> view);
    constexpr ArrayViewBase(const T* data, std::size_t size);
    constexpr ArrayViewBase(const T* start, const T* end);
    ArrayViewBase(const std::vector<T>& vec);
    constexpr ArrayViewBase(std::initializer_list<T> lst);
    //template <typename U = T>
    //ArrayViewBase(typename std::enable_if<std::is_same<U, uint8_t>::value, bmcl::Buffer>::type& buf);
    template <typename U = T>
    BMCL_CONSTEXPR_CPP17 ArrayViewBase(typename std::enable_if<std::is_same<U, char>::value, const char*>::type str);

    template <std::size_t N>
    constexpr static B fromStaticArray(const T(&data)[N]);

    constexpr static B empty();

    constexpr iterator begin() const;
    constexpr iterator end() const;

    constexpr reverse_iterator rbegin() const;
    constexpr reverse_iterator rend() const;

    constexpr const T* data() const;
    constexpr std::size_t size() const;
    constexpr bool isEmpty() const;

    // python-style slicing
    // sliceFrom - array[start:]
    // sliceFromBack - array[:-fromBack]
    // sliceTo - array[:end]
    // slice - array[start:end]

    B sliceFrom(std::size_t start) const;
    B sliceFromBack(std::size_t fromBack) const;
    B sliceTo(std::size_t end) const;
    B slice(std::size_t start, std::size_t end) const;

    std::vector<T> toStdVector() const;

    const T& operator[](std::size_t index) const;

    bool operator==(B other) const;
    bool operator!=(B other) const;
    B& operator=(B other);
    template <typename R>
    B& operator=(R&& other);

private:
    const T* _data;
    std::size_t _size;
};
}

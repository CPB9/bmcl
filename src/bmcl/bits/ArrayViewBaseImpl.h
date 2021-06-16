/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/Config.h"
#include "bmcl/bits/ArrayViewBaseDecl.h"
#include "bmcl/Assert.h"

#if defined(BMCL_PLATFORM_APPLE)
#include <string>
#else
#include <cstring>
#endif

namespace bmcl {

template <typename T, typename B>
constexpr inline ArrayViewBase<T, B>::ArrayViewBase()
    : _data(nullptr)
    , _size(0)
{
}

template <typename T, typename B>
template <std::size_t N>
constexpr inline B ArrayViewBase<T, B>::fromStaticArray(const T(&data)[N])
{
    return B(data, N);
}

template <typename T, typename B>
constexpr inline ArrayViewBase<T, B>::ArrayViewBase(const T* data, std::size_t size)
    : _data(data)
    , _size(size)
{
}

template <typename T, typename B>
constexpr inline ArrayViewBase<T, B>::ArrayViewBase(const T* start, const T* end)
    : _data(start)
    , _size(end - start)
{
}

template <typename T, typename B>
inline ArrayViewBase<T, B>::ArrayViewBase(const std::vector<T>& vec)
    : _data(vec.data())
    , _size(vec.size())
{
}

template <typename T, typename B>
constexpr inline ArrayViewBase<T, B>::ArrayViewBase(std::initializer_list<T> lst)
    : _data(lst.begin())
    , _size(lst.size())
{
}

template <typename T, typename B>
template <std::size_t N>
inline ArrayViewBase<T, B>::ArrayViewBase(const std::array<T, N>& array)
    : _data(array.data())
    , _size(N)
{
}

template <typename T, typename B>
template <std::size_t N>
inline ArrayViewBase<T, B>::ArrayViewBase(FixedArrayView<T, N> view)
    : _data(view.data())
    , _size(N)
{
}

//template <typename T, typename B>
//template <typename U>
//inline ArrayViewBase<T, B>::ArrayViewBase(typename std::enable_if<std::is_same<U, uint8_t>::value, bmcl::Buffer>::type& buf)
//    : ArrayViewBase<T, B>(buf.begin(), buf.size())
//{
//}

template <typename T, typename B>
template <typename U>
BMCL_CONSTEXPR_CPP17 inline ArrayViewBase<T, B>::ArrayViewBase(typename std::enable_if<std::is_same<U, char>::value, const char*>::type str)
    : _data(str)
    , _size(std::char_traits<char>::length(str))
{
}

template <typename T, typename B>
constexpr inline B ArrayViewBase<T, B>::empty()
{
    return B(nullptr, std::size_t(0));
}

template <typename T, typename B>
constexpr inline typename ArrayViewBase<T, B>::iterator ArrayViewBase<T, B>::begin() const
{
    return _data;
}

template <typename T, typename B>
constexpr inline typename ArrayViewBase<T, B>::iterator ArrayViewBase<T, B>::end() const
{
    return _data + _size;
}

template <typename T, typename B>
constexpr inline typename ArrayViewBase<T, B>::reverse_iterator ArrayViewBase<T, B>::rbegin() const
{
    return reverse_iterator(end());
}

template <typename T, typename B>
constexpr inline typename ArrayViewBase<T, B>::reverse_iterator ArrayViewBase<T, B>::rend() const
{
    return reverse_iterator(begin());
}

template <typename T, typename B>
constexpr inline bool ArrayViewBase<T, B>::isEmpty() const
{
    return _size == 0;
}

template <typename T, typename B>
inline std::vector<T> ArrayViewBase<T, B>::toStdVector() const
{
    return std::vector<T>(_data, _data + _size);
}

template <typename T, typename B>
constexpr inline const T* ArrayViewBase<T, B>::data() const
{
    return _data;
}

template <typename T, typename B>
constexpr inline std::size_t ArrayViewBase<T, B>::size() const
{
    return _size;
}

template <typename T, typename B>
inline B ArrayViewBase<T, B>::sliceFrom(std::size_t start) const
{
    BMCL_ASSERT(_size >= start);
    return B(_data + start, _size - start);
}

template <typename T, typename B>
inline B ArrayViewBase<T, B>::sliceFromBack(std::size_t fromBack) const
{
    BMCL_ASSERT(_size >= fromBack);
    return B(_data, _size - fromBack);
}

template <typename T, typename B>
inline B ArrayViewBase<T, B>::sliceTo(std::size_t end) const
{
    BMCL_ASSERT(_size >= end);
    return B(_data, end);
}

template <typename T, typename B>
inline B ArrayViewBase<T, B>::slice(std::size_t start, std::size_t end) const
{
    BMCL_ASSERT(_size >= end);
    BMCL_ASSERT(start <= end);
    return B(_data + start, end - start);
}

template <typename T, typename B>
inline const T& ArrayViewBase<T, B>::operator[](std::size_t index) const
{
    BMCL_ASSERT(index < _size);
    return _data[index];
}

template <typename T, typename B>
inline bool ArrayViewBase<T, B>::operator==(B other) const
{
    return (size() == other.size()) && (std::memcmp(data(), other.data(), size()) == 0);
}

template <typename T, typename B>
inline bool ArrayViewBase<T, B>::operator!=(B other) const
{
    return (size() != other.size()) || (std::memcmp(data(), other.data(), size()) != 0);
}

template <typename T, typename B>
inline B& ArrayViewBase<T, B>::operator=(B other)
{
    _data = other._data;
    _size = other._size;
    return *static_cast<B*>(this);
}

template <typename T, typename B>
template <typename R>
inline B& ArrayViewBase<T, B>::operator=(R&& other)
{
    B view(std::forward<R>(other));
    _data = view._data;
    _size = view._size;
    return *static_cast<B*>(this);
}
}

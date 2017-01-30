/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/Config.h"
#include "bmcl/Assert.h"

#include <array>
#include <cstddef>
#include <vector>
#include <stdint.h>

namespace bmcl {

template <typename T, size_t S>
class FixedArrayView {
public:
    typedef std::size_t size_type;
    typedef const T* iterator;
    typedef const T* const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;

    FixedArrayView(const T(&data)[S]);
    FixedArrayView(const std::array<T, S>& lst);

    static FixedArrayView<T, S> fromRawData(const T* data);

    iterator begin() const;
    iterator end() const;

    reverse_iterator rbegin() const;
    reverse_iterator rend() const;

    const T* data() const;
    std::size_t size() const;
    bool isEmpty() const;

    const T& operator[](std::size_t index) const;

private:
    FixedArrayView() = default;

    const T* _data;
};

template <typename T, size_t S>
inline FixedArrayView<T, S>::FixedArrayView(const T(&data)[S])
    : _data(data)
{
}

template <typename T, size_t S>
inline FixedArrayView<T, S>::FixedArrayView(const std::array<T, S>& lst)
    : _data(lst.data())
{
}

template <typename T, size_t S>
inline FixedArrayView<T, S> FixedArrayView<T, S>::fromRawData(const T* data)
{
    FixedArrayView<T, S> view;
    view._data = data;
    return view;
}

template <typename T, size_t S>
inline typename FixedArrayView<T, S>::iterator FixedArrayView<T, S>::begin() const
{
    return _data;
}

template <typename T, size_t S>
inline typename FixedArrayView<T, S>::iterator FixedArrayView<T, S>::end() const
{
    return _data + S;
}

template <typename T, size_t S>
inline typename FixedArrayView<T, S>::reverse_iterator FixedArrayView<T, S>::rbegin() const
{
    return reverse_iterator(rend());
}

template <typename T, size_t S>
inline typename FixedArrayView<T, S>::reverse_iterator FixedArrayView<T, S>::rend() const
{
    return reverse_iterator(rbegin());
}

template <typename T, size_t S>
inline const T* FixedArrayView<T, S>::data() const
{
    return _data;
}

template <typename T, size_t S>
inline std::size_t FixedArrayView<T, S>::size() const
{
    return S;
}

template <typename T, size_t S>
inline bool FixedArrayView<T, S>::isEmpty() const
{
    return S == 0;
}

template <typename T, size_t S>
inline const T& FixedArrayView<T, S>::operator[](std::size_t index) const
{
    BMCL_ASSERT(index < S);
    return _data[index];
}

template <typename T, typename B>
class ArrayViewBase {
public:
    typedef std::size_t size_type;
    typedef const T* iterator;
    typedef const T* const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;

    template <std::size_t N>
    ArrayViewBase(const T(&data)[N]);
    template <std::size_t N>
    ArrayViewBase(const std::array<T, N>& lst);
    ArrayViewBase(const T* data, std::size_t size);
    ArrayViewBase(const T* start, const T* end);
    ArrayViewBase(const std::vector<T>& vec);
    ArrayViewBase(const std::initializer_list<T>& lst);

    static B empty();

    iterator begin() const;
    iterator end() const;

    reverse_iterator rbegin() const;
    reverse_iterator rend() const;

    const T* data() const;
    std::size_t size() const;
    bool isEmpty() const;

    //template <std::size_t N>
    //void reset(const T(&data)[N]);
    template <std::size_t N>
    void reset(const std::array<T, N>& lst);
    void reset(const T* data, std::size_t size);
    void reset(const T* start, const T* end);
    void reset(const std::vector<T>& vec);
    void reset(const std::initializer_list<T>& lst);

    // python-style slicing
    // sliceFrom - array[start:]
    // sliceTo - array[:end]
    // slice - array[start:end]

    B sliceFrom(std::size_t start) const;
    B sliceTo(std::size_t end) const;
    B slice(std::size_t start, std::size_t end) const;

    std::vector<T> toStdVector() const;

    const T& operator[](std::size_t index) const;

private:
    const T* _data;
    std::size_t _size;
};

template <typename T, typename B>
template <std::size_t N>
inline ArrayViewBase<T, B>::ArrayViewBase(const T(&data)[N])
    : _data(data)
    , _size(N)
{
}

template <typename T, typename B>
inline ArrayViewBase<T, B>::ArrayViewBase(const T* data, std::size_t size)
    : _data(data)
    , _size(size)
{
}

template <typename T, typename B>
inline ArrayViewBase<T, B>::ArrayViewBase(const T* start, const T* end)
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
inline ArrayViewBase<T, B>::ArrayViewBase(const std::initializer_list<T>& lst)
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
inline B ArrayViewBase<T, B>::empty()
{
    return B(nullptr, std::size_t(0));
}

template <typename T, typename B>
inline typename ArrayViewBase<T, B>::iterator ArrayViewBase<T, B>::begin() const
{
    return _data;
}

template <typename T, typename B>
inline typename ArrayViewBase<T, B>::iterator ArrayViewBase<T, B>::end() const
{
    return _data + _size;
}

template <typename T, typename B>
inline typename ArrayViewBase<T, B>::reverse_iterator ArrayViewBase<T, B>::rbegin() const
{
    return reverse_iterator(end());
}

template <typename T, typename B>
inline typename ArrayViewBase<T, B>::reverse_iterator ArrayViewBase<T, B>::rend() const
{
    return reverse_iterator(begin());
}

template <typename T, typename B>
inline bool ArrayViewBase<T, B>::isEmpty() const
{
    return _size == 0;
}

template <typename T, typename B>
inline void ArrayViewBase<T, B>::reset(const T* data, std::size_t size)
{
    _data = data;
    _size = size;
}

template <typename T, typename B>
inline void ArrayViewBase<T, B>::reset(const T* start, const T* end)
{
    _data = start;
    _size = end - start;
}

template <typename T, typename B>
inline void ArrayViewBase<T, B>::reset(const std::vector<T>& vec)
{
    _data = vec.data();
    _size = vec.size();
}

template <typename T, typename B>
inline void ArrayViewBase<T, B>::reset(const std::initializer_list<T>& lst)
{
    _data = lst.begin();
    _size = lst.size();
}

template <typename T, typename B>
template <std::size_t N>
inline void ArrayViewBase<T, B>::reset(const std::array<T, N>& array)
{
    _data = array.data();
    _size = N;
}

template <typename T, typename B>
inline std::vector<T> ArrayViewBase<T, B>::toStdVector() const
{
    return std::vector<T>(_data, _data + _size);
}

template <typename T, typename B>
inline const T* ArrayViewBase<T, B>::data() const
{
    return _data;
}

template <typename T, typename B>
inline std::size_t ArrayViewBase<T, B>::size() const
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

template <typename T>
class ArrayView : public ArrayViewBase<T, ArrayView<T>> {
public:
    template <typename... A>
    ArrayView(A&&... args)
        : ArrayViewBase<T, ArrayView>(std::forward<A>(args)...)
    {
    }

    ArrayView(const std::initializer_list<T>& lst)
        : ArrayViewBase<T, ArrayView>(lst)
    {
    }
};

typedef ArrayView<uint8_t> Bytes;
}

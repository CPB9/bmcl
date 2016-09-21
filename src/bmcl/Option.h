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
#include "bmcl/AlignedUnion.h"
#include "bmcl/Utils.h"

#include <utility>

namespace bmcl {

template <typename T>
class Option {
public:
    Option();
    Option(NoneType);
    ~Option();
    Option(const T& value);
    Option(T&& value);
    Option(const Option& other);
    Option(Option&& other);

    template <typename... A>
    Option(InPlaceType, A&&... args);

    template <typename... A>
    void emplace(A&&... args);

    bool isSome() const;
    bool isNone() const;

    const T& unwrap() const;
    T& unwrap();

    void clear();

    template <class R>
#ifdef _MSC_VER
    T unwrapOr(R&& value) const;
#else
    T unwrapOr(R&& value) const&;

    template <class R>
    T unwrapOr(R&& value) && ;
#endif

    T take();

    Option& operator=(const Option& other);
    Option& operator=(Option&& other);
    Option& operator=(const T& value);
    Option& operator=(T&& value);

    const T* operator->() const;
    T* operator->();
    const T& operator*() const;
    T& operator*();

private:
    const T* asValue() const;
    T* asValue();

    AlignedUnion<T> _data;
    bool _isSome;
};

template <typename T>
inline const T* Option<T>::asValue() const
{
    return reinterpret_cast<const T*>(&_data);
}

template <typename T>
inline T* Option<T>::asValue()
{
    return reinterpret_cast<T*>(&_data);
}

template <typename T>
inline Option<T>::Option()
    : _isSome(false)
{
}

template <typename T>
inline Option<T>::Option(NoneType)
    : _isSome(false)
{
}

template <typename T>
template <typename... A>
inline Option<T>::Option(InPlaceType, A&&... args)
    : _isSome(true)
{
    new (asValue()) T(std::forward<A>(args)...);
}

template <typename T>
Option<T>::~Option()
{
    if (_isSome) {
        asValue()->~T();
    }
}

template <typename T>
inline Option<T>::Option(const T& value)
    : _isSome(true)
{
    new (asValue()) T(value);
}

template <typename T>
inline Option<T>::Option(T&& value)
    : _isSome(true)
{
    new (asValue()) T(std::forward<T>(value));
}

template <typename T>
Option<T>::Option(const Option<T>& other)
    : _isSome(other._isSome)
{
    if (_isSome) {
        new (asValue()) T(*other.asValue());
    }
}

template <typename T>
Option<T>::Option(Option<T>&& other)
    : _isSome(other._isSome)
{
    if (_isSome) {
        new (asValue()) T(std::move(*other.asValue()));
        other.clear();
    }
}

template <typename T>
inline bool Option<T>::isSome() const
{
    return _isSome;
}

template <typename T>
inline bool Option<T>::isNone() const
{
    return !_isSome;
}

template <typename T>
inline const T& Option<T>::unwrap() const
{
    BMCL_ASSERT(_isSome);
    return *asValue();
}

template <typename T>
inline T& Option<T>::unwrap()
{
    BMCL_ASSERT(_isSome);
    return *asValue();
}

template<typename T>
void Option<T>::clear()
{
    if (_isSome) {
        _isSome = false;
        asValue()->~T();
    }
}

template <typename T>
template <typename R>
#ifdef _MSC_VER
T Option<T>::unwrapOr(R&& value) const
#else
T Option<T>::unwrapOr(R&& value) const &
#endif
{
    if (_isSome) {
        return unwrap();
    }
    return std::forward<R>(value);
}

#ifndef _MSC_VER
template <typename T>
template <typename R>
T Option<T>::unwrapOr(R&& value) &&
{
    if (_isSome) {
        return take();
    }
    return std::forward<R>(value);
}
#endif

template <typename T>
T Option<T>::take()
{
    BMCL_ASSERT(_isSome);
    T data = std::move(*asValue());
    clear();
    return data;
}

template <typename T>
Option<T>& Option<T>::operator=(const Option<T>& other)
{
    if (other._isSome) {
        if (_isSome) {
            *asValue() = *other.asValue();
        } else {
            _isSome = true;
            new (asValue()) T(*other.asValue());
        }
    } else {
        clear();
    }
    return *this;
}

template <typename T>
Option<T>& Option<T>::operator=(Option<T>&& other)
{
    if (other._isSome) {
        if (_isSome) {
            *asValue() = std::move(*other.asValue());
        } else {
            _isSome = true;
            new (asValue()) T(std::move(*other.asValue()));
        }
        other.clear();
    } else {
        clear();
    }
    return *this;
}

template <typename T>
Option<T>& Option<T>::operator=(const T& value)
{
    if (_isSome) {
        *asValue() = value;
    } else {
        _isSome = true;
        new (asValue()) T(value);
    }
    return *this;
}

template <typename T>
template <typename... A>
void Option<T>::emplace(A&&... args)
{
    if (_isSome) {
        *asValue() = T(std::forward<A>(args)...);
    } else {
        _isSome = true;
        new (asValue()) T(std::forward<A>(args)...);
    }
}

template <typename T>
Option<T>& Option<T>::operator=(T&& value)
{
    if (_isSome) {
        *asValue() = std::forward<T>(value);
    } else {
        _isSome = true;
        new (asValue()) T(std::forward<T>(value));
    }
    return *this;
}

template <typename T>
inline const T* Option<T>::operator->() const
{
    BMCL_ASSERT(_isSome);
    return asValue();
}

template <typename T>
inline T* Option<T>::operator->()
{
    BMCL_ASSERT(_isSome);
    return asValue();
}

template <typename T>
inline const T& Option<T>::operator*() const
{
    BMCL_ASSERT(_isSome);
    return *asValue();
}

template <typename T>
inline T& Option<T>::operator*()
{
    BMCL_ASSERT(_isSome);
    return *asValue();
}

template <typename T>
class Option<T&> {
public:
    Option(NoneType);
    Option(std::nullptr_t);
    Option(T& value);
    Option(const Option& other);
    Option(Option&& other);

    bool isSome() const;
    bool isNone() const;

    const T& unwrap() const;
    T& unwrap();

    void clear();
    void emplace(T& value);

    const T& unwrapOr(const T& value) const;

    Option& operator=(const Option& other);
    Option& operator=(Option&& other);

    const T* operator->() const;
    T* operator->();
    const T& operator*() const;
    T& operator*();

private:
    T* _data;
};

template <typename T>
inline Option<T&>::Option(NoneType)
    : _data(nullptr)
{
}

template <typename T>
inline Option<T&>::Option(std::nullptr_t)
    : _data(nullptr)
{
}

template <typename T>
inline Option<T&>::Option(T& value)
    : _data(&value)
{
}

template <typename T>
inline Option<T&>::Option(const Option<T&>& other)
    : _data(other._data)
{
}

template <typename T>
inline Option<T&>::Option(Option<T&>&& other)
    : _data(other._data)
{
    other._data = nullptr;
}

template <typename T>
inline bool Option<T&>::isSome() const
{
    return _data != nullptr;
}

template <typename T>
inline bool Option<T&>::isNone() const
{
    return _data == nullptr;
}

template <typename T>
inline const T& Option<T&>::unwrap() const
{
    BMCL_ASSERT(isSome());
    return *_data;
}

template <typename T>
inline T& Option<T&>::unwrap()
{
    BMCL_ASSERT(isSome());
    return *_data;
}

template <typename T>
inline void Option<T&>::clear()
{
    _data = nullptr;
}

template <typename T>
inline void Option<T&>::emplace(T& value)
{
    _data = &value;
}

template <typename T>
const T& Option<T&>::unwrapOr(const T& value) const
{
    if (_data) {
        return *_data;
    }
    return value;
}

template <typename T>
Option<T&>& Option<T&>::operator=(const Option<T&>& other)
{
    _data = other._data;
    return *this;
}

template <typename T>
Option<T&>& Option<T&>::operator=(Option<T&>&& other)
{
    _data = other._data;
    other._data = nullptr;
    return *this;
}

template <typename T>
inline const T* Option<T&>::operator->() const
{
    BMCL_ASSERT(isSome());
    return _data;
}

template <typename T>
inline T* Option<T&>::operator->()
{
    BMCL_ASSERT(isSome());
    return _data;
}

template <typename T>
inline const T& Option<T&>::operator*() const
{
    BMCL_ASSERT(isSome());
    return *_data;
}

template <typename T>
inline T& Option<T&>::operator*()
{
    BMCL_ASSERT(isSome());
    return *_data;
}


template <typename T>
bool operator==(const Option<T>& left, const Option<T>& right)
{
    if (left.isSome() && right.isSome()) {
        return left.unwrap() == right.unwrap();
    }
    return left.isSome() == right.isSome();
}

template <typename T>
inline bool operator!=(const Option<T>& left, const Option<T>& right)
{
    return !operator==(left, right);
}

}

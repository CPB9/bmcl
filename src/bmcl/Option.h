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

    template <typename U>
#if defined(_MSC_VER) && (_MSC_VER < 1900)
    BMCL_DLL_INLINE Option<U> into() const;
#else
    BMCL_DLL_INLINE Option<U> into() const&;

    template <typename U>
    BMCL_DLL_INLINE Option<U> into() &&;
#endif

    const T& unwrap() const;
    T& unwrap();

    void clear();

    template <class R>
#if defined(_MSC_VER) && (_MSC_VER < 1900)
    BMCL_DLL_INLINE T unwrapOr(R&& value) const;
#else
    BMCL_DLL_INLINE T unwrapOr(R&& value) const&;

    template <class R>
    BMCL_DLL_INLINE T unwrapOr(R&& value) && ;
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
    new (asValue()) T(std::move(value));
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
template <typename U>
#if defined(_MSC_VER) && (_MSC_VER < 1900)
BMCL_DLL_INLINE Option<U> Option<T>::into()
#else
BMCL_DLL_INLINE Option<U> Option<T>::into() const&
#endif
{
    if (isSome()) {
        return Option<U>(unwrap());
    }
    return bmcl::None;
}

#if !defined(_MSC_VER) || (_MSC_VER >= 1900)
template <typename T>
template <typename U>
BMCL_DLL_INLINE Option<U> Option<T>::into() &&
{
    if (isSome()) {
        return Option<U>(take());
    }
    return bmcl::None;
}
#endif


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
#if defined(_MSC_VER) && (_MSC_VER < 1900)
BMCL_DLL_INLINE T Option<T>::unwrapOr(R&& value) const
#else
BMCL_DLL_INLINE T Option<T>::unwrapOr(R&& value) const&
#endif
{
    if (_isSome) {
        return unwrap();
    }
    return std::forward<R>(value);
}

#if !defined(_MSC_VER) || (_MSC_VER >= 1900)
template <typename T>
template <typename R>
BMCL_DLL_INLINE T Option<T>::unwrapOr(R&& value) &&
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
        asValue()->~T();
    } else {
        _isSome = true;
    }
    new (asValue()) T(std::forward<A>(args)...);
}

template <typename T>
Option<T>& Option<T>::operator=(T&& value)
{
    if (_isSome) {
        *asValue() = std::move(value);
    } else {
        _isSome = true;
        new (asValue()) T(std::move(value));
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
    Option();
    Option(NoneType);
    Option(std::nullptr_t);
    Option(T& value);
    Option(const Option& other);
    Option(Option&& other);

    template <typename C>
    Option(const Option<C&>& other);
    template <typename C>
    Option(Option<C&>&& other);

    const T* data() const;
    T* data();

    bool isSome() const;
    bool isNone() const;

    const T& unwrap() const;
    T& unwrap();

    void clear();
    void emplace(T& value);

    const T& unwrapOr(const T& value) const;

    Option& operator=(const Option& other);
    Option& operator=(Option&& other);
    Option& operator=(T& other);

    const T* operator->() const;
    T* operator->();
    const T& operator*() const;
    T& operator*();

private:
    T* _data;
};

template <typename T>
inline Option<T&>::Option()
    : _data(nullptr)
{
}

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
template <typename C>
inline Option<T&>::Option(const Option<C&>& other)
    : _data(other.data())
{
}

template <typename T>
template <typename C>
inline Option<T&>::Option(Option<C&>&& other)
    : _data(other.data())
{
    other.clear();
}

template <typename T>
inline T* Option<T&>::data()
{
    return _data;
}

template <typename T>
inline const T* Option<T&>::data() const
{
    return _data;
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
Option<T&>& Option<T&>::operator=(T& other)
{
    _data = &other;
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


template <typename T, typename U>
bool operator==(const Option<T>& left, const Option<U>& right)
{
    if (left.isSome() && right.isSome()) {
        return left.unwrap() == right.unwrap();
    }
    return left.isSome() == right.isSome();
}

template <typename T, typename U>
inline bool operator!=(const Option<T>& left, const Option<U>& right)
{
    return !operator==(left, right);
}

template <typename T, typename U>
bool operator==(const Option<T>& left, const U& right)
{
    if (left.isSome()) {
        return left.unwrap() == right;
    }
    return false;
}

template <typename T, typename U>
inline bool operator!=(const Option<T>& left, const U& right)
{
    return !operator==(left, right);
}

template <typename T, typename U>
bool operator==(const T& left, const bmcl::Option<U>& right)
{
    if (right.isSome()) {
        return left == right.unwrap();
    }
    return false;
}

template <typename T, typename U>
inline bool operator!=(const T& left, const Option<U>& right)
{
    return !operator==(left, right);
}

}

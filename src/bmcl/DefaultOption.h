/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/Config.h"
#include "bmcl/Utils.h"

#include <cassert>

namespace bmcl {

template <typename T, typename D, D defaultValue>
class DefaultOption {
public:
    DefaultOption();
    DefaultOption(NoneType);
    ~DefaultOption();

    DefaultOption(const T& value);
    DefaultOption(T&& value);
    DefaultOption(const DefaultOption& other);
    DefaultOption(DefaultOption&& other);

    template <typename C>
    DefaultOption(const DefaultOption<C, D, defaultValue>& other);
    template <typename C>
    DefaultOption(DefaultOption<C, D, defaultValue>&& other);

    template <typename... A>
    DefaultOption(InPlaceType, A&&... args);

    template <typename... A>
    void emplace(A&&... args);

    bool isSome() const;
    bool isNone() const;

    const T& unwrap() const;
    T& unwrap();

    template <typename R>
    T unwrapOr(R&& value) const&;

    template <typename R>
    T&& unwrapOr(R&& value) &&;

    void clear();

    T take();

    const T& data() const;
    T& data();

    DefaultOption& operator=(const DefaultOption& other);
    DefaultOption& operator=(DefaultOption&& other);
    DefaultOption& operator=(const T& value);
    DefaultOption& operator=(T&& value);
    DefaultOption& operator=(NoneType);

    const T& operator->() const;
    T& operator->();
    const T& operator*() const;
    T& operator*();

private:
    T _value;
};

template <typename T, typename D, D defaultValue>
DefaultOption<T, D, defaultValue>::DefaultOption()
    : _value(defaultValue)
{
}

template <typename T, typename D, D defaultValue>
DefaultOption<T, D, defaultValue>::DefaultOption(NoneType)
    : _value(defaultValue)
{
}

template <typename T, typename D, D defaultValue>
DefaultOption<T, D, defaultValue>::~DefaultOption()
{
}

template <typename T, typename D, D defaultValue>
DefaultOption<T, D, defaultValue>::DefaultOption(const T& value)
    : _value(value)
{
}

template <typename T, typename D, D defaultValue>
DefaultOption<T, D, defaultValue>::DefaultOption(T&& value)
    : _value(std::move(value))
{
}

template <typename T, typename D, D defaultValue>
DefaultOption<T, D, defaultValue>::DefaultOption(const DefaultOption& other)
    : _value(other._value)
{
}

template <typename T, typename D, D defaultValue>
DefaultOption<T, D, defaultValue>::DefaultOption(DefaultOption&& other)
    : _value(std::move(other._value))
{
    other._value = defaultValue;
}

template <typename T, typename D, D defaultValue>
template <typename C>
DefaultOption<T, D, defaultValue>::DefaultOption(const DefaultOption<C, D, defaultValue>& other)
    : _value(other.data())
{
}

template <typename T, typename D, D defaultValue>
template <typename C>
DefaultOption<T, D, defaultValue>::DefaultOption(DefaultOption<C, D, defaultValue>&& other)
    : _value(std::move(other._value))
{
    other.clear();
}

template <typename T, typename D, D defaultValue>
template <typename... A>
DefaultOption<T, D, defaultValue>::DefaultOption(InPlaceType, A&&... args)
    : _value(std::forward<A>(args)...)
{
}

template <typename T, typename D, D defaultValue>
template <typename... A>
void DefaultOption<T, D, defaultValue>::emplace(A&&... args)
{
    _value.~T();
    new (&_value) T(std::forward<A>(args)...);
}

template <typename T, typename D, D defaultValue>
bool DefaultOption<T, D, defaultValue>::isSome() const
{
    return _value != defaultValue;
}

template <typename T, typename D, D defaultValue>
bool DefaultOption<T, D, defaultValue>::isNone() const
{
    return _value == defaultValue;
}

template <typename T, typename D, D defaultValue>
const T& DefaultOption<T, D, defaultValue>::unwrap() const
{
    assert(isSome());
    return _value;
}

template <typename T, typename D, D defaultValue>
T& DefaultOption<T, D, defaultValue>::unwrap()
{
    assert(isSome());
    return _value;
}

template <typename T, typename D, D defaultValue>
template <typename R>
T DefaultOption<T, D, defaultValue>::unwrapOr(R&& value) const&
{
    if (isSome()) {
        return unwrap();
    }
    return std::forward<R>(value);
}

template <typename T, typename D, D defaultValue>
template <typename R>
T&& DefaultOption<T, D, defaultValue>::unwrapOr(R&& value) &&
{
    if (isSome()) {
        return std::move(_value);
    }
    return std::forward<R>(value);
}

template <typename T, typename D, D defaultValue>
void DefaultOption<T, D, defaultValue>::clear()
{
    _value = defaultValue;
}

template <typename T, typename D, D defaultValue>
T DefaultOption<T, D, defaultValue>::take()
{
    assert(isSome());
    T val = _value;
    _value = defaultValue;
    return val;
}

template <typename T, typename D, D defaultValue>
const T& DefaultOption<T, D, defaultValue>::data() const
{
    return _value;
}

template <typename T, typename D, D defaultValue>
T& DefaultOption<T, D, defaultValue>::data()
{
    return _value;
}

template <typename T, typename D, D defaultValue>
DefaultOption<T, D, defaultValue>& DefaultOption<T, D, defaultValue>::operator=(const DefaultOption& other)
{
    _value = other._value;
    return *this;
}

template <typename T, typename D, D defaultValue>
DefaultOption<T, D, defaultValue>& DefaultOption<T, D, defaultValue>::operator=(DefaultOption&& other)
{
    _value = std::move(other._value);
    other._value = defaultValue;
    return *this;
}

template <typename T, typename D, D defaultValue>
DefaultOption<T, D, defaultValue>& DefaultOption<T, D, defaultValue>::operator=(const T& value)
{
    _value = value;
    return *this;
}

template <typename T, typename D, D defaultValue>
DefaultOption<T, D, defaultValue>& DefaultOption<T, D, defaultValue>::operator=(T&& value)
{
    _value = std::move(value);
    return *this;
}

template <typename T, typename D, D defaultValue>
DefaultOption<T, D, defaultValue>& DefaultOption<T, D, defaultValue>::operator=(NoneType)
{
    _value = defaultValue;
    return *this;
}

template <typename T, typename D, D defaultValue>
const T& DefaultOption<T, D, defaultValue>::operator->() const
{
    assert(isSome());
    return _value;
}

template <typename T, typename D, D defaultValue>
T& DefaultOption<T, D, defaultValue>::operator->()
{
    assert(isSome());
    return _value;
}

template <typename T, typename D, D defaultValue>
const T& DefaultOption<T, D, defaultValue>::operator*() const
{
    assert(isSome());
    return _value;
}

template <typename T, typename D, D defaultValue>
T& DefaultOption<T, D, defaultValue>::operator*()
{
    assert(isSome());
    return _value;
}

template <typename T, typename U, typename D, D def>
bool operator==(const DefaultOption<T, D, def>& left, const DefaultOption<U, D, def>& right)
{
    return left.data() == right.data();
}

template <typename T, typename U, typename D, D def>
bool operator==(const DefaultOption<T, D, def>& left, const U& right)
{
    return left.data() == right;
}

template <typename T, typename U, typename D, D def>
bool operator==(const T& left, const DefaultOption<U, D, def>& right)
{
    return left == right.data();
}

template <typename T, typename U, typename D, D def>
bool operator!=(const DefaultOption<T, D, def>& left, const DefaultOption<U, D, def>& right)
{
    return left.data() != right.data();
}

template <typename T, typename U, typename D, D def>
bool operator!=(const DefaultOption<T, D, def>& left, const U& right)
{
    return left.data() != right;
}

template <typename T, typename U, typename D, D def>
bool operator!=(const T& left, const DefaultOption<U, D, def>& right)
{
    return left != right.data();
}
}

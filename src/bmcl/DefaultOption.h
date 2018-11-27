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

template <typename D, D def>
struct DefaultOptionDescriptor {
    static constexpr D defaultValue()
    {
        return def;
    }

    template <typename T>
    static constexpr bool isDefault(const T& value)
    {
        return value == def;
    }
};

template <typename T, typename D>
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
    DefaultOption(const DefaultOption<C, D>& other);
    template <typename C>
    DefaultOption(DefaultOption<C, D>&& other);

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
    T unwrapOr(R&& value) &&;

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

template <typename T, typename D>
DefaultOption<T, D>::DefaultOption()
    : _value(D::defaultValue())
{
}

template <typename T, typename D>
DefaultOption<T, D>::DefaultOption(NoneType)
    : _value(D::defaultValue())
{
}

template <typename T, typename D>
DefaultOption<T, D>::~DefaultOption()
{
}

template <typename T, typename D>
DefaultOption<T, D>::DefaultOption(const T& value)
    : _value(value)
{
}

template <typename T, typename D>
DefaultOption<T, D>::DefaultOption(T&& value)
    : _value(std::move(value))
{
}

template <typename T, typename D>
DefaultOption<T, D>::DefaultOption(const DefaultOption& other)
    : _value(other._value)
{
}

template <typename T, typename D>
DefaultOption<T, D>::DefaultOption(DefaultOption&& other)
    : _value(std::move(other._value))
{
    other._value = D::defaultValue();
}

template <typename T, typename D>
template <typename C>
DefaultOption<T, D>::DefaultOption(const DefaultOption<C, D>& other)
    : _value(other.data())
{
}

template <typename T, typename D>
template <typename C>
DefaultOption<T, D>::DefaultOption(DefaultOption<C, D>&& other)
    : _value(std::move(other._value))
{
    other.clear();
}

template <typename T, typename D>
template <typename... A>
DefaultOption<T, D>::DefaultOption(InPlaceType, A&&... args)
    : _value(std::forward<A>(args)...)
{
}

template <typename T, typename D>
template <typename... A>
void DefaultOption<T, D>::emplace(A&&... args)
{
    _value.~T();
    new (&_value) T(std::forward<A>(args)...);
}

template <typename T, typename D>
bool DefaultOption<T, D>::isSome() const
{
    return !D::isDefault(_value);
}

template <typename T, typename D>
bool DefaultOption<T, D>::isNone() const
{
    return D::isDefault(_value);
}

template <typename T, typename D>
const T& DefaultOption<T, D>::unwrap() const
{
    assert(isSome());
    return _value;
}

template <typename T, typename D>
T& DefaultOption<T, D>::unwrap()
{
    assert(isSome());
    return _value;
}

template <typename T, typename D>
template <typename R>
T DefaultOption<T, D>::unwrapOr(R&& value) const&
{
    if (isSome()) {
        return unwrap();
    }
    return std::forward<R>(value);
}

template <typename T, typename D>
template <typename R>
T DefaultOption<T, D>::unwrapOr(R&& value) &&
{
    if (isSome()) {
        return std::move(_value);
    }
    return std::forward<R>(value);
}

template <typename T, typename D>
void DefaultOption<T, D>::clear()
{
    _value = D::defaultValue();
}

template <typename T, typename D>
T DefaultOption<T, D>::take()
{
    assert(isSome());
    T val = _value;
    _value = D::defaultValue();
    return val;
}

template <typename T, typename D>
const T& DefaultOption<T, D>::data() const
{
    return _value;
}

template <typename T, typename D>
T& DefaultOption<T, D>::data()
{
    return _value;
}

template <typename T, typename D>
DefaultOption<T, D>& DefaultOption<T, D>::operator=(const DefaultOption& other)
{
    _value = other._value;
    return *this;
}

template <typename T, typename D>
DefaultOption<T, D>& DefaultOption<T, D>::operator=(DefaultOption&& other)
{
    _value = std::move(other._value);
    other._value = D::defaultValue();
    return *this;
}

template <typename T, typename D>
DefaultOption<T, D>& DefaultOption<T, D>::operator=(const T& value)
{
    _value = value;
    return *this;
}

template <typename T, typename D>
DefaultOption<T, D>& DefaultOption<T, D>::operator=(T&& value)
{
    _value = std::move(value);
    return *this;
}

template <typename T, typename D>
DefaultOption<T, D>& DefaultOption<T, D>::operator=(NoneType)
{
    _value = D::defaultValue();
    return *this;
}

template <typename T, typename D>
const T& DefaultOption<T, D>::operator->() const
{
    assert(isSome());
    return _value;
}

template <typename T, typename D>
T& DefaultOption<T, D>::operator->()
{
    assert(isSome());
    return _value;
}

template <typename T, typename D>
const T& DefaultOption<T, D>::operator*() const
{
    assert(isSome());
    return _value;
}

template <typename T, typename D>
T& DefaultOption<T, D>::operator*()
{
    assert(isSome());
    return _value;
}

template <typename T, typename U, typename D>
bool operator==(const DefaultOption<T, D>& left, const DefaultOption<U, D>& right)
{
    return left.data() == right.data();
}

template <typename T, typename U, typename D>
bool operator==(const DefaultOption<T, D>& left, const U& right)
{
    return left.data() == right;
}

template <typename T, typename U, typename D>
bool operator==(const T& left, const DefaultOption<U, D>& right)
{
    return left == right.data();
}

template <typename T, typename U, typename D>
bool operator!=(const DefaultOption<T, D>& left, const DefaultOption<U, D>& right)
{
    return left.data() != right.data();
}

template <typename T, typename U, typename D>
bool operator!=(const DefaultOption<T, D>& left, const U& right)
{
    return left.data() != right;
}

template <typename T, typename U, typename D>
bool operator!=(const T& left, const DefaultOption<U, D>& right)
{
    return left != right.data();
}
}

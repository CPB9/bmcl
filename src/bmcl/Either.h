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

#include <new>
#include <utility>

namespace bmcl {

template <typename T, typename E>
class Either {
public:
    Either(const T& first);
    Either(T&& first);
    Either(const E& second);
    Either(E&& second);
    Either(const Either& other);
    Either(Either&& other);
    ~Either();

    bool isFirst() const;
    bool isSecond() const;
    const T& unwrapFirst() const;
    T& unwrapFirst();
    const E& unwrapSecond() const;
    E& unwrapSecond();
    T&& takeFirst();
    E&& takeSecond();

    //TODO: op=
    Either& operator=(const Either& other);
    Either& operator=(Either&& other);
//     Either& operator=(const T& first);
//     Either& operator=(T&& first);
//     Either& operator=(const E& second);
//     Either& operator=(E&& second);

private:
    const T* asFirst() const;
    T* asFirst();
    const E* asSecond() const;
    E* asSecond();

#ifdef _MSC_VER
    struct TEContainer {
        T t;
        E e;
    };

    typename std::aligned_union<sizeof(TEContainer), T, E>::type _data;
#else
    template <typename S>
    static constexpr S max(S t, S r)
    {
        return t > r ? t : r;
    }

    typename std::aligned_storage<max(sizeof(T), sizeof(E)), max(alignof(T), alignof(E))>::type _data;
#endif
    bool _isFirst;
};

template <typename T, typename E>
inline const T* Either<T, E>::asFirst() const
{
    return reinterpret_cast<const T*>(&_data);
}

template <typename T, typename E>
inline T* Either<T, E>::asFirst()
{
    return reinterpret_cast<T*>(&_data);
}

template <typename T, typename E>
inline const E* Either<T, E>::asSecond() const
{
    return reinterpret_cast<const E*>(&_data);
}

template <typename T, typename E>
inline E* Either<T, E>::asSecond()
{
    return reinterpret_cast<E*>(&_data);
}

template <typename T, typename E>
inline Either<T, E>::Either(const T& value)
    : _isFirst(true)
{
    new (asFirst()) T(value);
}

template <typename T, typename E>
inline Either<T, E>::Either(T&& value)
    : _isFirst(true)
{
    new (asFirst()) T(std::move(value));
}

template <typename T, typename E>
inline Either<T, E>::Either(const E& error)
    : _isFirst(false)
{
    new (asSecond()) E(error);
}

template <typename T, typename E>
inline Either<T, E>::Either(E&& error)
    : _isFirst(false)
{
    new (asSecond()) E(std::move(error));
}

template <typename T, typename E>
inline Either<T, E>::Either(const Either<T, E>& other)
{
    _isFirst = other._isFirst;
    if (_isFirst) {
        new (asFirst()) T(*other.asFirst());
    } else {
        new (asSecond()) E(*other.asSecond());
    }
}

template <typename T, typename E>
inline Either<T, E>::Either(Either<T, E>&& other)
{
    _isFirst = other._isFirst;
    if (_isFirst) {
        new (asFirst()) T(std::move(*other.asFirst()));
    } else {
        new (asSecond()) E(std::move(*other.asSecond()));
    }
}

template <typename T, typename E>
inline Either<T, E>::~Either()
{
    if (_isFirst) {
        asFirst()->~T();
    } else {
        asSecond()->~E();
    }
}

template <typename T, typename E>
inline bool Either<T, E>::isFirst() const
{
    return _isFirst;
}

template <typename T, typename E>
inline bool Either<T, E>::isSecond() const
{
    return !_isFirst;
}

template <typename T, typename E>
inline const T& Either<T, E>::unwrapFirst() const
{
    BMCL_ASSERT(_isFirst);
    return *asFirst();
}

template <typename T, typename E>
inline T& Either<T, E>::unwrapFirst()
{
    BMCL_ASSERT(_isFirst);
    return *asFirst();
}

template <typename T, typename E>
inline const E& Either<T, E>::unwrapSecond() const
{
    BMCL_ASSERT(!_isFirst);
    return *asSecond();
}

template <typename T, typename E>
inline E& Either<T, E>::unwrapSecond()
{
    BMCL_ASSERT(!_isFirst);
    return *asSecond();
}

template <typename T, typename E>
inline T&& Either<T, E>::takeFirst()
{
    BMCL_ASSERT(_isFirst);
    return std::move(*asFirst());
}

template <typename T, typename E>
inline E&& Either<T, E>::takeSecond()
{
    BMCL_ASSERT(!_isFirst);
    return std::move(*asSecond());
}

template <typename T, typename E>
inline Either<T, E>& Either<T, E>::operator=(const Either<T, E>& other)
{
    if (other._isFirst) {
        if (_isFirst) {
            *asFirst() = *other.asFirst();
        } else {
            _isFirst = true;
            asSecond()->~E();
            new (asFirst()) T(*other.asFirst());
        }
    } else {
        if (_isFirst) {
            _isFirst = false;
            asFirst()->~T();
            new (asSecond()) E(*other.asSecond());
        } else {
            *asSecond() = std::move(*other.asSecond());
        }
    }
    return *this;
}

template <typename T, typename E>
inline Either<T, E>& Either<T, E>::operator=(Either<T, E>&& other)
{
    if (other._isFirst) {
        if (_isFirst) {
            *asFirst() = std::move(*other.asFirst());
        } else {
            _isFirst = true;
            asSecond()->~E();
            new (asFirst()) T(std::move(*other.asFirst()));
        }
    } else {
        if (_isFirst) {
            _isFirst = false;
            asFirst()->~T();
            new (asSecond()) E(std::move(*other.asSecond()));
        } else {
            *asSecond() = std::move(*other.asSecond());
        }
    }
    return *this;
}
}

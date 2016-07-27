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

#include <new>
#include <utility>

namespace bmcl {

template <typename T, typename E>
class Either {
public:
    Either(const T& first);
    Either(T&& first);
    template <typename U = E>
    Either(const enableIfNotVoid<U>& second);
    template <typename U = E>
    Either(enableIfNotVoid<U>&& second);
    template <typename U = E, typename = enableIfVoid<U>>
    Either();
    Either(const Either& other);
    Either(Either&& other);
    ~Either();

    template <typename... A>
    Either(InPlaceFirstType, A&&... args);
    template <typename... A>
    Either(InPlaceSecondType, A&&... args);

    bool isFirst() const;
    bool isSecond() const;

    const T& unwrapFirst() const;
    T& unwrapFirst();
    T&& takeFirst();

    template <typename U = E>
    const enableIfNotVoid<U>& unwrapSecond() const;
    template <typename U = E>
    enableIfNotVoid<U>& unwrapSecond();
    template <typename U = E>
    enableIfNotVoid<U>&& takeSecond();

    //TODO: op=
    Either& operator=(const Either& other);
    Either& operator=(Either&& other);
//     Either& operator=(const T& first);
//     Either& operator=(T&& first);
//     Either& operator=(const E& second);
//     Either& operator=(E&& second);

private:
    void copyConstruct(const Either<T, E>& other, std::true_type isVoid);
    void copyConstruct(const Either<T, E>& other, std::false_type isVoid);
    void moveConstruct(Either<T, E>&& other, std::true_type isVoid);
    void moveConstruct(Either<T, E>&& other, std::false_type isVoid);
    Either& copyEquality(const Either<T, E>& other, std::true_type isVoid);
    Either& copyEquality(const Either<T, E>& other, std::false_type isVoid);
    Either& moveEquality(Either<T, E>&& other, std::true_type isVoid);
    Either& moveEquality(Either<T, E>&& other, std::false_type isVoid);
    void destruct(std::true_type isVoid);
    void destruct(std::false_type isVoid);
    const T* asFirst() const;
    T* asFirst();
    const E* asSecond() const;
    E* asSecond();
    typename std::conditional<std::is_void<E>::value, AlignedUnion<T>, AlignedUnion<T, E>>::type _data;
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
template <typename U>
inline Either<T, E>::Either(const enableIfNotVoid<U>& error)
    : _isFirst(false)
{
    new (asSecond()) E(error);
}

template <typename T, typename E>
template <typename U>
inline Either<T, E>::Either(enableIfNotVoid<U>&& error)
    : _isFirst(false)
{
    new (asSecond()) E(std::move(error));
}

template <typename T, typename E>
template <typename U, typename>
inline Either<T, E>::Either()
    : _isFirst(false)
{
}

template <typename T, typename E>
void Either<T, E>::copyConstruct(const Either<T, E>& other, std::true_type)
{
    _isFirst = other._isFirst;
    if (_isFirst) {
        new (asFirst()) T(*other.asFirst());
    }
}

template <typename T, typename E>
void Either<T, E>::copyConstruct(const Either<T, E>& other, std::false_type)
{
    _isFirst = other._isFirst;
    if (_isFirst) {
        new (asFirst()) T(*other.asFirst());
    } else {
        new (asSecond()) E(*other.asSecond());
    }
}

template <typename T, typename E>
inline Either<T, E>::Either(const Either<T, E>& other)
{
    copyConstruct(other, std::is_void<E>());
}

template <typename T, typename E>
void Either<T, E>::moveConstruct(Either<T, E>&& other, std::true_type)
{
    _isFirst = other._isFirst;
    if (_isFirst) {
        new (asFirst()) T(std::move(*other.asFirst()));
    }
}

template <typename T, typename E>
void Either<T, E>::moveConstruct(Either<T, E>&& other, std::false_type)
{
    _isFirst = other._isFirst;
    if (_isFirst) {
        new (asFirst()) T(std::move(*other.asFirst()));
    } else {
        new (asSecond()) E(std::move(*other.asSecond()));
    }
}

template <typename T, typename E>
inline Either<T, E>::Either(Either<T, E>&& other)
{
    moveConstruct(std::move(other), std::is_void<E>());
}

template <typename T, typename E>
void Either<T, E>::destruct(std::true_type)
{
    if (_isFirst) {
        asFirst()->~T();
    }
}

template <typename T, typename E>
void Either<T, E>::destruct(std::false_type)
{
    if (_isFirst) {
        asFirst()->~T();
    } else {
        asSecond()->~E();
    }
}

template <typename T, typename E>
inline Either<T, E>::~Either()
{
    destruct(std::is_void<E>());
}

template <typename T, typename E>
template <typename... A>
inline Either<T, E>::Either(InPlaceFirstType, A&&... args)
    : _isFirst(true)
{
    new (asFirst()) T(std::forward<A>(args)...);
}

template <typename T, typename E>
template <typename... A>
inline Either<T, E>::Either(InPlaceSecondType, A&&... args)
    : _isFirst(false)
{
    new (asSecond()) E(std::forward<A>(args)...);
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
template <typename U>
inline const enableIfNotVoid<U>& Either<T, E>::unwrapSecond() const
{
    BMCL_ASSERT(!_isFirst);
    return *asSecond();
}

template <typename T, typename E>
template <typename U>
inline enableIfNotVoid<U>& Either<T, E>::unwrapSecond()
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
template <typename U>
inline enableIfNotVoid<U>&& Either<T, E>::takeSecond()
{
    BMCL_ASSERT(!_isFirst);
    return std::move(*asSecond());
}

template <typename T, typename E>
Either<T, E>& Either<T, E>::copyEquality(const Either<T, E>& other, std::true_type)
{
    if (other._isFirst) {
        if (_isFirst) {
            *asFirst() = *other.asFirst();
        } else {
            _isFirst = true;
            new (asFirst()) T(*other.asFirst());
        }
    } else {
        if (_isFirst) {
            _isFirst = false;
            asFirst()->~T();
        }
    }
    return *this;
}

template <typename T, typename E>
Either<T, E>& Either<T, E>::copyEquality(const Either<T, E>& other, std::false_type)
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
inline Either<T, E>& Either<T, E>::operator=(const Either<T, E>& other)
{
    return copyEquality(other, std::is_void<E>());
}

template <typename T, typename E>
Either<T, E>& Either<T, E>::moveEquality(Either<T, E>&& other, std::false_type)
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

template <typename T, typename E>
Either<T, E>& Either<T, E>::moveEquality(Either<T, E>&& other, std::true_type)
{
    if (other._isFirst) {
        if (_isFirst) {
            *asFirst() = std::move(*other.asFirst());
        } else {
            _isFirst = true;
            new (asFirst()) T(std::move(*other.asFirst()));
        }
    } else {
        if (_isFirst) {
            _isFirst = false;
            asFirst()->~T();
        }
    }
    return *this;
}

template <typename T, typename E>
inline Either<T, E>& Either<T, E>::operator=(Either<T, E>&& other)
{
    return moveEquality(std::move(other), std::is_void<E>());
}
}

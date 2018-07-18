/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/Config.h"
#include "bmcl/Either.h"
#include "bmcl/Option.h"

namespace bmcl {

template <typename T, typename E>
class Result : private Either<T, E> {
public:
    Result(const T& value);
    Result(T&& value);
    template <typename U = E>
    Result(const enableIfNotVoid<U>& error);
    template <typename U = E>
    Result(enableIfNotVoid<U>&& error);
    template <typename U = E, typename = enableIfVoid<U>>
    Result();
    Result(const Result& error);
    Result(Result&& error);

    bool isOk() const;
    bool isErr() const;
    const T& unwrap() const;
    T& unwrap();
    T&& take();

    template <class R>
#if defined(_MSC_VER) && (_MSC_VER < 1900)
    BMCL_DLL_INLINE T unwrapOr(R&& value) const;
#else
    BMCL_DLL_INLINE T unwrapOr(R&& value) const&;

    template <class R>
    BMCL_DLL_INLINE T unwrapOr(R&& value) && ;
#endif

    template <typename U = E>
    const enableIfNotVoid<U>& unwrapErr() const;
    template <typename U = E>
    enableIfNotVoid<U>& unwrapErr();
    template <typename U = E>
    enableIfNotVoid<U>&& takeErr();
    Option<T> unwrapOption() const;
    Option<T> takeOption();
    template <typename U = E>
    enableIfNotVoid<U, Option<E>> unwrapErrOption() const;
    template <typename U = E>
    enableIfNotVoid<U, Option<E>> takeErrOption();

    Result& operator=(const Result& other);
    Result& operator=(Result&& other);
};

template <typename T, typename E>
inline Result<T, E>::Result(const T& value)
    : Either<T, E>::Either(value)
{
}

template <typename T, typename E>
inline Result<T, E>::Result(T&& value)
    : Either<T, E>::Either(std::move(value))
{
}

template <typename T, typename E>
template <typename U>
inline Result<T, E>::Result(const enableIfNotVoid<U>& error)
    : Either<T, E>::Either(error)
{
}

template <typename T, typename E>
template <typename U>
inline Result<T, E>::Result(enableIfNotVoid<U>&& error)
    : Either<T, E>::Either(std::move(error))
{
}

template <typename T, typename E>
template <typename U, typename>
inline Result<T, E>::Result()
{
}

template <typename T, typename E>
inline Result<T, E>::Result(const Result& result)
    : Either<T, E>::Either(result)
{
}

template <typename T, typename E>
inline Result<T, E>::Result(Result&& result)
    : Either<T, E>::Either(std::move(result))
{
}

template <typename T, typename E>
inline bool Result<T, E>::isOk() const
{
    return Either<T, E>::isFirst();
}

template <typename T, typename E>
inline bool Result<T, E>::isErr() const
{
    return Either<T, E>::isSecond();
}

template <typename T, typename E>
inline const T& Result<T, E>::unwrap() const
{
    return Either<T, E>::unwrapFirst();
}

template <typename T, typename E>
inline T& Result<T, E>::unwrap()
{
    return Either<T, E>::unwrapFirst();
}

template <typename T, typename E>
template <typename U>
inline const enableIfNotVoid<U>& Result<T, E>::unwrapErr() const
{
    return Either<T, E>::unwrapSecond();
}

template <typename T, typename E>
template <typename U>
inline enableIfNotVoid<U>& Result<T, E>::unwrapErr()
{
    return Either<T, E>::unwrapSecond();
}

template <typename T, typename E>
inline T&& Result<T, E>::take()
{
    return Either<T, E>::takeFirst();
}

template <typename T, typename E>
template <typename R>
#if defined(_MSC_VER) && (_MSC_VER < 1900)
BMCL_DLL_INLINE T Result<T, E>::unwrapOr(R&& value) const
#else
BMCL_DLL_INLINE T Result<T, E>::unwrapOr(R&& value) const&
#endif
{
    if (isOk()) {
        return unwrap();
    }
    return std::forward<R>(value);
}

#if !defined(_MSC_VER) || (_MSC_VER >= 1900)
template <typename T, typename E>
template <typename R>
BMCL_DLL_INLINE T Result<T, E>::unwrapOr(R&& value) &&
{
    if (isOk()) {
        return take();
    }
    return std::forward<R>(value);
}
#endif

template <typename T, typename E>
template <typename U>
inline enableIfNotVoid<U>&& Result<T, E>::takeErr()
{
    return Either<T, E>::takeSecond();
}

template <typename T, typename E>
inline Option<T> Result<T, E>::unwrapOption() const
{
    if (isOk())
        return Either<T, E>::unwrapFirst();
    return bmcl::None;
}

template <typename T, typename E>
inline Option<T> Result<T, E>::takeOption()
{
    if (isOk())
        return Either<T, E>::takeFirst();
    return bmcl::None;
}

template <typename T, typename E>
template <typename U>
inline enableIfNotVoid<U, Option<E>> Result<T, E>::unwrapErrOption() const
{
    if (isErr())
        return Either<T, E>::unwrapSecond();
    return bmcl::None;
}

template <typename T, typename E>
template <typename U>
inline enableIfNotVoid<U, Option<E>> Result<T, E>::takeErrOption()
{
    if (isErr())
        return Either<T, E>::takeSecond();
    return bmcl::None;
}

template <typename T, typename E>
inline Result<T, E>& Result<T, E>::operator=(const Result<T, E>& other)
{
    return static_cast<Result<T, E>&>(Either<T, E>::operator=(other));
}

template <typename T, typename E>
inline Result<T, E>& Result<T, E>::operator=(Result<T, E>&& other)
{
    return static_cast<Result<T, E>&>(Either<T, E>::operator=(std::move(other)));
}
}

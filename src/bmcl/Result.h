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

namespace bmcl {

template <typename T, typename E>
class Result : private Either<T, E> {
public:
    Result(const T& value);
    Result(T&& value);
    Result(const E& error);
    Result(E&& error);
    Result(const Result& error);
    Result(Result&& error);

    bool isOk() const;
    bool isErr() const;
    const T& unwrap() const;
    T& unwrap();
    const E& unwrapErr() const;
    E& unwrapErr();
    T&& take();
    E&& takeErr();

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
inline Result<T, E>::Result(const E& error)
    : Either<T, E>::Either(error)
{
}

template <typename T, typename E>
inline Result<T, E>::Result(E&& error)
    : Either<T, E>::Either(std::move(error))
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
inline const E& Result<T, E>::unwrapErr() const
{
    return Either<T, E>::unwrapSecond();
}

template <typename T, typename E>
inline E& Result<T, E>::unwrapErr()
{
    return Either<T, E>::unwrapSecond();
}

template <typename T, typename E>
inline T&& Result<T, E>::take()
{
    return Either<T, E>::takeFirst();
}

template <typename T, typename E>
inline E&& Result<T, E>::takeErr()
{
    return Either<T, E>::takeSecond();
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

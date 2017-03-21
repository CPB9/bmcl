/*
 * Copyright (c) 2014-2015 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <memory>
#include "bmcl/NonNullUniquePtr.h"
#include "bmcl/Utils.h"

namespace bmcl
{

template <typename R, typename T, typename... A>
inline std::unique_ptr<R> makeUniqueSubType(A&&... args)
{
    return std::unique_ptr<R>(new T(std::forward<A>(args)...));
}

template <class T>
inline std::weak_ptr<T> makeWeak(const std::shared_ptr<T> & shared)
{
    return std::weak_ptr<T>(shared);
};

template<class T, class ...Args>
NonNullUniquePtr<T> makeNonNullUniquePtr(Args && ...args)
{
    return NonNullUniquePtr<T>(makeUnique<T>(std::forward<Args>(args)...));
};

template <class To, class From, class Deleter>
std::unique_ptr<To, Deleter> dynamicCast(std::unique_ptr<From, Deleter> && p)
{
    if (To* cast = dynamic_cast<To*>(p.get()))
    {
        std::unique_ptr<To, Deleter> result(cast, std::move(p.get_deleter()));
        p.release();
        return result;
    }
    return std::unique_ptr<To, Deleter>(nullptr);
}

template<class T>
class NonNullUniquePtr;

template<class To, class From>
NonNullUniquePtr<To> staticCast(NonNullUniquePtr<From> && p)
{
    return NonNullUniquePtr<To>(std::move(p.ptr()));
}

}

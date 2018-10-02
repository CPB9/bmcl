#pragma once

#include "bmcl/Config.h"
#include "bmcl/Rc.h"

#include <utility>

namespace bmcl {

template <typename T, typename... A>
inline Rc<T> makeRc(A&&... args)
{
    return new T(std::forward<A>(args)...);
}

template <typename T>
inline Rc<T> wrapRc(T* ptr)
{
    return ptr;
}
}

/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <cstddef>

#include "bmcl/Config.h"
#include "bmcl/Option.h"
#include "bmcl/Utils.h"
#include "bmcl/Assert.h"

namespace bmcl
{

template <typename T>
class OptionPtr
{
public:
    OptionPtr();
    OptionPtr(std::nullptr_t);
    OptionPtr(NoneType);
    ~OptionPtr();
    OptionPtr(T* value);
    OptionPtr(const OptionPtr<T*>& other);
    OptionPtr(OptionPtr<T*>&& other);

    bool isSome() const;
    bool isNone() const;

    const T* unwrap() const;
    T* unwrap();

    void clear();

    T* take();

    OptionPtr& operator=(const OptionPtr& other);
    OptionPtr& operator=(OptionPtr&& other);
    OptionPtr& operator=(T* value);

    const T* operator->() const;
    T* operator->();
    const T& operator*() const;
    T& operator*();

    template <class R>
#ifdef _MSC_VER
    T* unwrapOr(R* value) const;
#else
    T* unwrapOr(R* value) const&;

    template <class R>
    T* unwrapOr(R* value) &&;
#endif

private:

    T* _data;

};

template<class T>
OptionPtr<T>::OptionPtr()
        : _data(nullptr)
{

}

template<class T>
OptionPtr<T>::OptionPtr(std::nullptr_t)
        : OptionPtr()
{

}

template<class T>
OptionPtr<T>::OptionPtr(NoneType)
        : OptionPtr()
{

}

template<class T>
OptionPtr<T>::~OptionPtr()
{

}

template<class T>
OptionPtr<T>::OptionPtr(T* value)
        : _data(value)
{

}

template<class T>
OptionPtr<T>::OptionPtr(const OptionPtr<T*>& other)
        : _data(other._data)
{

}

template<class T>
OptionPtr<T>::OptionPtr(OptionPtr<T*>&& other)
{
    other.clear();
}

template<class T>
bool OptionPtr<T>::isSome() const
{
    return _data != nullptr;
}

template<class T>
bool OptionPtr<T>::isNone() const
{
    return _data == nullptr;
}

template<class T>
const T* OptionPtr<T>::unwrap() const
{
    return _data;
}

template<class T>
T* OptionPtr<T>::unwrap()
{
    return _data;
}

template<class T>
void OptionPtr<T>::clear()
{
    _data = nullptr;
}

template<class T>
T* OptionPtr<T>::take()
{
    T* res = _data; clear();
    return res;
}

template<class T>
OptionPtr<T>& OptionPtr<T>::operator=(const OptionPtr<T>& other)
{
    _data = other._data;
    return *this;
}

template<class T>
OptionPtr<T>& OptionPtr<T>::operator=(OptionPtr<T>&& other)
{
    _data = other._data;
    other.clear();
    return *this;
}

template<class T>
OptionPtr<T>& OptionPtr<T>::operator=(T* value)
{
    _data = value;
    return *this;
}

template<class T>
inline const T* OptionPtr<T>::operator->() const
{
    return _data;
}

template<class T>
inline T* OptionPtr<T>::operator->()
{
    return _data;
}

template<class T>
inline const T& OptionPtr<T>::operator*() const
{
    BMCL_ASSERT(_data != nullptr);
    return *_data;
}

template<class T>
inline T& OptionPtr<T>::operator*()
{
    BMCL_ASSERT(_data != nullptr);
    return *_data;
}

#ifdef _MSC_VER

template<class T>
template <class R>
T* OptionPtr<T>::unwrapOr(R* value) const
{
    return _data != nullptr? _data : value;
}

#else

template<class T>
template <class R>
inline T* OptionPtr<T>::unwrapOr(R* value) const&
{
    return _data != nullptr ? _data : value;
}

template<class T>
template <class R>
inline T* OptionPtr<T>::unwrapOr(R* value) &&
{
    if (_data != nullptr)
    {
        T* res = _data;
        clear();
        return res;
    }
    return value;
}

#endif

}

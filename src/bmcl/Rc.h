/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <bmcl/Config.h>

#include <cstddef>

namespace bmcl {

template <typename T>
class Rc {
public:
    using value_type = T;

    Rc();
    Rc(T* ptr);
    Rc(const Rc& other);
    Rc(Rc&& other);
    template <typename U>
    Rc(const Rc<U>& other);
    ~Rc();

    void reset();
    void reset(T* other);
    T* get() const;
    T* detach();

    bool isNull() const;

    void swap(Rc& other);

    Rc& operator=(Rc&& other);
    Rc& operator=(const Rc& other);
    Rc& operator=(T* other);
    template <typename U>
    Rc& operator=(const Rc<U>& other);

    T& operator*() const;
    T* operator->() const;

private:
    T* _ptr;
};

template <typename T>
inline Rc<T>::Rc()
    : _ptr(0)
{
}

template <typename T>
Rc<T>::Rc(T* ptr)
    : _ptr(ptr)
{
    if (ptr != 0) {
        bmclRcAddRef(_ptr);
    }
}

template <typename T>
Rc<T>::Rc(const Rc& other)
    : _ptr(other._ptr)
{
    if(_ptr != 0) {
        bmclRcAddRef(_ptr);
    }
}

template <typename T>
inline Rc<T>::Rc(Rc<T>&& other)
    : _ptr(other._ptr)
{
    other._ptr = 0;
}

template <typename T>
template <typename U>
Rc<T>::Rc(const Rc<U>& other)
    : _ptr(other.get())
{
    if(_ptr != 0) {
        bmclRcAddRef(_ptr);
    }
}

template <typename T>
Rc<T>::~Rc()
{
    if(_ptr != 0) {
        bmclRcRelease( _ptr );
    }
}

template <typename T>
void Rc<T>::reset()
{
    Rc<T>().swap(*this);
}

template <typename T>
void Rc<T>::reset(T* other)
{
    Rc<T>(other).swap(*this);
}

template <typename T>
inline T* Rc<T>::get() const
{
    return _ptr;
}

template <typename T>
T* Rc<T>::detach()
{
    T* rv = _ptr;
    _ptr = 0;
    return rv;
}

template <typename T>
void Rc<T>::swap(Rc<T>& other)
{
    T* temp = _ptr;
    _ptr = other._ptr;
    other._ptr = temp;
}

template <typename T>
template <typename U>
Rc<T>& Rc<T>::operator=(const Rc<U>& other)
{
    Rc<T>(other).swap(*this);
    return *this;
}

template <typename T>
Rc<T>& Rc<T>::operator=(Rc<T>&& other)
{
    Rc<T>(static_cast<Rc<T>&&>(other)).swap(*this);
    return *this;
}

template <typename T>
Rc<T>& Rc<T>::operator=(const Rc<T>& other)
{
    Rc<T>(other).swap(*this);
    return *this;
}

template <typename T>
Rc<T>& Rc<T>::operator=(T* other)
{
    Rc<T>(other).swap(*this);
    return *this;
}

template <typename T>
inline T& Rc<T>::operator*() const
{
    return *_ptr;
}

template <typename T>
inline T* Rc<T>::operator->() const
{
    return _ptr;
}

template <typename T>
inline bool Rc<T>::isNull() const
{
    return _ptr == 0;
}

template <typename T, typename U>
inline bool operator==(const Rc<T>& left, const Rc<U>& right)
{
    return left.get() == right.get();
}

template <typename T, typename U>
inline bool operator!=(const Rc<T>& left, const Rc<U>& right)
{
    return left.get() != right.get();
}

template <typename T, typename U>
inline bool operator==(const Rc<T>& left, U* right)
{
    return left.get() == right;
}

template <typename T, typename U>
inline bool operator!=(const Rc<T>& left, U* right)
{
    return left.get() != right;
}

template <typename T>
inline bool operator==(const Rc<T>& left, std::nullptr_t)
{
    return left.get() == nullptr;
}

template <typename T>
inline bool operator!=(const Rc<T>& left, std::nullptr_t)
{
    return left.get() != nullptr;
}

template <typename T, typename U>
inline bool operator==(T* left, const Rc<U>& right)
{
    return left == right.get();
}

template <typename T, typename U>
inline bool operator!=(T* left, const Rc<U>& right)
{
    return left != right.get();
}

template <typename T>
void swap(Rc<T>& left, Rc<T>& right)
{
    left.swap(right);
}

template <typename T>
T* get_pointer(const Rc<T>& ptr)
{
    return ptr.get();
}

template <typename T, typename U>
Rc<T> static_pointer_cast(const Rc<U>& ptr)
{
    return static_cast<T*>(ptr.get());
}

template <typename T, typename U>
Rc<T> const_pointer_cast(const Rc<U>& ptr)
{
    return const_cast<T*>(ptr.get());
}

template <typename T, typename U>
Rc<T> dynamic_pointer_cast(const Rc<U>& ptr)
{
    return dynamic_cast<T*>(ptr.get());
}
}

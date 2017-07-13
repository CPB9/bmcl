/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/Config.h"
#include "bmcl/Bytes.h"

#include <atomic>
#include <cstdlib>
#include <cstddef>
#include <cassert>
#include <cstring>

namespace bmcl {

class SharedBytes {
public:
    SharedBytes();
    ~SharedBytes();

    static SharedBytes create(bmcl::Bytes view);
    static SharedBytes create(const uint8_t* data, std::size_t size);
    static SharedBytes create(std::size_t size);

    SharedBytes clone() const;
    void swap(SharedBytes& other);
    uint8_t* data();

    bmcl::Bytes view() const;
    const uint8_t* data() const;
    std::size_t size() const;

    SharedBytes(const SharedBytes& other);
    SharedBytes(SharedBytes&& other);

    SharedBytes& operator=(const SharedBytes& other);
    SharedBytes& operator=(SharedBytes&& other);

private:
    struct SharedBytesData {
        void incRef();
        void decRef();

        std::atomic<std::size_t> rc;
        std::size_t size;
    };

    static constexpr std::size_t dataOffset = sizeof(SharedBytesData);

    SharedBytes(SharedBytesData* cont);

    static SharedBytesData* allocContainer(std::size_t size);

    SharedBytesData* _cont;
};

inline void SharedBytes::SharedBytesData::incRef()
{
    rc.fetch_add(1, std::memory_order_relaxed);
}

inline void SharedBytes::SharedBytesData::decRef()
{
    if (rc.fetch_sub(1, std::memory_order_release) == 1) {
        std::atomic_thread_fence(std::memory_order_acquire);
        std::free(this);
    }
}

inline SharedBytes::SharedBytes(SharedBytesData* cont)
    : _cont(cont)
{
}

inline SharedBytes::SharedBytes(const SharedBytes& other)
    : _cont(other._cont)
{
    if (_cont != nullptr) {
        other._cont->incRef();
    }
}

inline SharedBytes::SharedBytes(SharedBytes&& other)
    : _cont(other._cont)
{
    other._cont = nullptr;
}

inline SharedBytes::SharedBytes()
    : _cont(nullptr)
{
}

inline SharedBytes::~SharedBytes()
{
    if (_cont) {
        _cont->decRef();
    }
}

inline SharedBytes SharedBytes::create(bmcl::Bytes view)
{
    return create(view.data(), view.size());
}

inline SharedBytes::SharedBytesData* SharedBytes::allocContainer(std::size_t size)
{
    std::size_t totalSize = dataOffset + size;
    void* allocated = std::malloc(totalSize);
    SharedBytesData* cont = (SharedBytesData*)allocated;
    cont->rc.store(1, std::memory_order_relaxed);
    cont->size = size;
    return cont;
}

inline SharedBytes SharedBytes::create(const uint8_t* data, std::size_t size)
{
    SharedBytesData* cont = allocContainer(size);
    std::memcpy((uint8_t*)cont + dataOffset, data, size);
    return SharedBytes(cont);
}

inline SharedBytes SharedBytes::create(std::size_t size)
{
    SharedBytesData* cont = allocContainer(size);
    return SharedBytes(cont);
}

inline SharedBytes SharedBytes::clone() const
{
    return SharedBytes::create(data(), size());
}

inline void SharedBytes::swap(SharedBytes& other)
{
    SharedBytesData* temp = _cont;
    _cont = other._cont;
    other._cont = temp;
}

inline bmcl::Bytes SharedBytes::view() const
{
    return bmcl::Bytes(data(), size());
}

inline uint8_t* SharedBytes::data()
{
    return (uint8_t*)_cont + dataOffset;
}

inline const uint8_t* SharedBytes::data() const
{
    return (const uint8_t*)_cont + dataOffset;
}

inline std::size_t SharedBytes::size() const
{
    return _cont->size;
}

inline SharedBytes& SharedBytes::operator=(const SharedBytes& other)
{
    SharedBytes(other).swap(*this);
    return *this;
}

inline SharedBytes& SharedBytes::operator=(SharedBytes&& other)
{
    SharedBytes(std::move(other)).swap(*this);
    return *this;
}
}

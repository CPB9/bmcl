/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "bmcl/SharedBytes.h"
#include "bmcl/Bytes.h"

#include <cassert>
#include <cstdlib>
#include <cstring>

namespace bmcl {

struct SharedBytes::SharedBytesData {
    void incRef();
    void decRef();

    std::atomic<std::size_t> rc;
    std::size_t size;
};

constexpr const std::size_t _dataOffset = sizeof(SharedBytes::SharedBytesData);

void SharedBytes::SharedBytesData::incRef()
{
    rc.fetch_add(1, std::memory_order_relaxed);
}

void SharedBytes::SharedBytesData::decRef()
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

SharedBytes::SharedBytes(const SharedBytes& other)
    : _cont(other._cont)
{
    if (_cont) {
        other._cont->incRef();
    }
}

SharedBytes::SharedBytes(SharedBytes&& other)
    : _cont(other._cont)
{
    other._cont = nullptr;
}


SharedBytes::~SharedBytes()
{
    if (_cont) {
        _cont->decRef();
    }
}

SharedBytes SharedBytes::create(bmcl::Bytes view)
{
    return create(view.data(), view.size());
}

SharedBytes::SharedBytesData* SharedBytes::allocContainer(std::size_t size)
{
    std::size_t totalSize = _dataOffset + size;
    void* allocated = std::malloc(totalSize);
    SharedBytesData* cont = (SharedBytesData*)allocated;
    cont->rc.store(1, std::memory_order_relaxed);
    cont->size = size;
    return cont;
}

SharedBytes SharedBytes::create(const uint8_t* data, std::size_t size)
{
    SharedBytesData* cont = allocContainer(size);
    std::memcpy((uint8_t*)cont + _dataOffset, data, size);
    return SharedBytes(cont);
}

SharedBytes SharedBytes::create(std::size_t size)
{
    SharedBytesData* cont = allocContainer(size);
    return SharedBytes(cont);
}

SharedBytes SharedBytes::clone() const
{
    if (_cont) {
        return SharedBytes::create((const uint8_t*)_cont + _dataOffset, _cont->size);
    }
    return SharedBytes();
}

void SharedBytes::swap(SharedBytes& other)
{
    SharedBytesData* temp = _cont;
    _cont = other._cont;
    other._cont = temp;
}

bmcl::Bytes SharedBytes::view() const
{
    return bmcl::Bytes(data(), size());
}

uint8_t* SharedBytes::data()
{
    if (_cont) {
        return (uint8_t*)_cont + _dataOffset;
    }
    return nullptr;
}

const uint8_t* SharedBytes::data() const
{
    if (_cont) {
        return (const uint8_t*)_cont + _dataOffset;
    }
    return nullptr;
}

std::size_t SharedBytes::size() const
{
    if (_cont) {
        return _cont->size;
    }
    return 0;
}

bool SharedBytes::isEmpty() const
{
    if (_cont) {
        return _cont->size == 0;
    }
    return true;
}

SharedBytes& SharedBytes::operator=(const SharedBytes& other)
{
    SharedBytes(other).swap(*this);
    return *this;
}

SharedBytes& SharedBytes::operator=(SharedBytes&& other)
{
    SharedBytes(std::move(other)).swap(*this);
    return *this;
}
}

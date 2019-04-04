/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/Config.h"
#include "bmcl/Fwd.h"

#include <atomic>
#include <cstddef>

namespace bmcl {

class BMCL_EXPORT SharedBytes {
public:
    struct SharedBytesData;

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

    bool isNull() const;
    bool isEmpty() const;

    SharedBytes(const SharedBytes& other);
    SharedBytes(SharedBytes&& other);

    SharedBytes& operator=(const SharedBytes& other);
    SharedBytes& operator=(SharedBytes&& other);

private:

    SharedBytes(SharedBytesData* cont);

    static SharedBytesData* allocContainer(std::size_t size);

    SharedBytesData* _cont;
};

inline SharedBytes::SharedBytes()
    : _cont(nullptr)
{
}

inline bool SharedBytes::isNull() const
{
    return _cont == nullptr;
}
}

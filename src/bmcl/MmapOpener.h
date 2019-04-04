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

#include <cstddef>
#include <cstdint>

#ifdef _WIN32
# ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#  undef WIN32_LEAN_AND_MEAN
# else
#  include <windows.h>
# endif
#else
# include <sys/mman.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <unistd.h>
#endif

namespace bmcl {

class BMCL_EXPORT MmapOpener {
public:
#ifdef _WIN32
    using FdType = HANDLE;
#else
    using FdType = int;
#endif

    MmapOpener();
    MmapOpener(const MmapOpener& other) = delete;
    MmapOpener(MmapOpener&& other);
    ~MmapOpener();

    MmapOpener& operator=(const MmapOpener& other) = delete;
    MmapOpener& operator=(MmapOpener&& other);

    bool open(const char* path);
    bool close();

    bool isOpen() const;

    const std::uint8_t* data() const;
    std::size_t size() const;
    Bytes view() const;

    const std::uint8_t* begin() const;
    const std::uint8_t* end() const;

private:
    void clearInternalData();
    void unmapAndClose();

    FdType _fd;
#ifdef _WIN32
    FdType _mapHandle;
#endif
    const std::uint8_t* _data;
    std::size_t _size;
};

inline bool MmapOpener::isOpen() const
{
    return _data != nullptr;
}

inline const std::uint8_t* MmapOpener::data() const
{
    return _data;
}

inline std::size_t MmapOpener::size() const
{
    return _size;
}

inline const std::uint8_t* MmapOpener::begin() const
{
    return _data;
}

inline const std::uint8_t* MmapOpener::end() const
{
    return _data + _size;
}
}

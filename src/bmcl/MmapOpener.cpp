/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "bmcl/MmapOpener.h"
#include "bmcl/Bytes.h"
#if defined(BMCL_PLATFORM_MSVC)
#include <xstring>
#endif

namespace bmcl {

#ifdef _WIN32
static const HANDLE invalidFd = INVALID_HANDLE_VALUE;
#else
static constexpr int invalidFd = -1;
#endif

MmapOpener::MmapOpener()
    : _fd(invalidFd)
#ifdef _WIN32
    , _mapHandle(invalidFd)
#endif
    , _data(nullptr)
    , _size(0)
{
}

MmapOpener::MmapOpener(MmapOpener&& other)
    : _fd(other._fd)
#ifdef _WIN32
    , _mapHandle(invalidFd)
#endif
    , _data(other._data)
    , _size(other._size)
{
    other.clearInternalData();
}

MmapOpener::~MmapOpener()
{
    unmapAndClose();
}

bool MmapOpener::open(const char* path)
{
    unmapAndClose();

#ifdef _WIN32
    // using CreateFileW to avoid file name length limitation
    std::size_t pathLen = std::strlen(path);
    std::wstring wPath(path, path + pathLen);

    _fd = ::CreateFileW(wPath.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
                        nullptr);
    if (_fd == invalidFd) {
        return false;
    }

    LARGE_INTEGER sizeStruct;
    if (!::GetFileSizeEx(_fd, &sizeStruct)) {
        ::CloseHandle(_fd);
        _fd = invalidFd;
        return false;
    }
    _size = sizeStruct.QuadPart;

    std::uint64_t size64 = _size;

    _mapHandle = ::CreateFileMappingW(_fd, nullptr, PAGE_READONLY, DWORD(size64 >> 32),
                                      DWORD(size64 & 0x00000000ffffffffull), nullptr);

    if (_mapHandle == invalidFd) {
        ::CloseHandle(_fd);
        _fd = invalidFd;
        return false;
    }

    _data = (const std::uint8_t*)::MapViewOfFile(_mapHandle, FILE_MAP_READ, 0, 0, 0);
    if (!_data) {
        ::CloseHandle(_mapHandle);
        _mapHandle = invalidFd;
        ::CloseHandle(_fd);
        _fd = invalidFd;
        return false;
    }
#else
    int fd = ::open(path, O_RDONLY);
    if (fd < 0) {
        return false;
    }
    _fd = fd;

    struct stat st;
    if (::fstat(_fd, &st) < 0) {
        ::close(fd);
        _fd = -1;
        return false;
    }

    _size = st.st_size;

    _data = (const std::uint8_t*)::mmap(nullptr, _size, PROT_READ, MAP_PRIVATE, _fd, 0);
    if (_data == MAP_FAILED) {
        ::close(fd);
        _fd = -1;
        return false;
    }
#endif
    return true;
}

bool MmapOpener::close()
{
    if (isOpen()) {
        unmapAndClose();
        return true;
    }
    return false;
}

bmcl::MmapOpener& MmapOpener::operator=(bmcl::MmapOpener&& other)
{
    unmapAndClose();
    _fd = other._fd;
#ifdef _WIN32
    _mapHandle = other._mapHandle;
#endif
    _data = other._data;
    _size = other._size;
    other.clearInternalData();
    return *this;
}

void MmapOpener::clearInternalData()
{
    _fd = invalidFd;
#ifdef _WIN32
    _mapHandle = invalidFd;
#endif
    _data = nullptr;
    _size = 0;
}

void MmapOpener::unmapAndClose()
{
    if (isOpen()) {
#ifdef _WIN32
        ::UnmapViewOfFile(_data);
        ::CloseHandle(_mapHandle);
        ::CloseHandle(_fd);
#else
        ::munmap((void*)_data, _size);
        ::close(_fd);
#endif
        clearInternalData();
    }
}

Bytes MmapOpener::view() const
{
    return Bytes(_data, _size);
}
}

/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "bmcl/Utils.h"
#include "bmcl/Result.h"
#include "bmcl/Buffer.h"

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <string>

namespace bmcl {

class FileGuard {
public:
    FileGuard(std::FILE* file)
        : _file(file)
    {
    }

    ~FileGuard()
    {
        std::fclose(_file);
    }

private:
    std::FILE* _file;
};

template <typename T>
Result<T, int> readFile(const char* path)
{
    std::FILE* file = std::fopen(path, "rb");
    if (!file) {
        return errno;
    }

    FileGuard guard(file);
    if (std::fseek(file, 0, SEEK_END)) {
        return errno;
    }

    long size = std::ftell(file);

    if (size == -1) {
        return errno;
    }

    if (std::fseek(file, 0, SEEK_SET)) {
        return errno;
    }

    T buf;
    buf.resize(size);

    if (std::fread(&buf[0], 1, size, file) != std::size_t(size)) {
        return errno;
    }

    return std::move(buf);
}

Result<std::string, int> readFileIntoString(const char* path)
{
    return readFile<std::string>(path);
}

Result<Buffer, int> readFileIntoBuffer(const char* path)
{
    return readFile<Buffer>(path);
}
}

/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "bmcl/FileUtils.h"
#include "bmcl/Assert.h"
#include "bmcl/Result.h"
#include "bmcl/Option.h"
#include "bmcl/Buffer.h"
#include "bmcl/SharedBytes.h"

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <string>
#include <mutex>
#include <cinttypes>

#if defined(BMCL_PLATFORM_WINDOWS)
# include <windows.h>
# include <shlwapi.h>
#elif defined(BMCL_PLATFORM_UNIX)
# include <sys/types.h>
# include <unistd.h>
# include <limits.h>
# include <libgen.h>
#elif defined(BMCL_PLATFORM_APPLE)
#include <unistd.h>
#include <libgen.h>
#include <mach-o/dyld.h>
#else
    #error "unsupported platform"
#endif

namespace bmcl {

class FileGuard {
public:
    explicit FileGuard(std::FILE* file)
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

template <typename T, typename C>
Result<T, int> readFile(const char* path, C&& create)
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

    T buf = create(size);

    if (std::fread((void*)buf.data(), 1, size, file) != std::size_t(size)) {
        return errno;
    }

    return buf;
}

Result<std::string, int> readFileIntoString(const char* path)
{
    return readFile<std::string>(path, [](std::size_t size) {
        return std::string(size, '\0');
    });
}

Result<Buffer, int> readFileIntoBuffer(const char* path)
{
    return readFile<Buffer>(path, [](std::size_t size) {
        return Buffer::createWithUnitializedData(size);
    });
}

Result<SharedBytes, int> readFileIntoBytes(const char* path)
{
    return readFile<SharedBytes>(path, [](std::size_t size) {
        return SharedBytes::create(size);
    });
}

std::uint64_t applicationPid()
{
#if defined(BMCL_PLATFORM_WINDOWS)
    return GetCurrentProcessId();
#elif defined(BMCL_PLATFORM_UNIX)
    return getpid();
#elif defined(BMCL_PLATFORM_APPLE)
    return getpid();
#endif
}

static std::once_flag appPathInitialized;
static Option<std::string> appPath;
static Option<std::string> appDirPath;

#define MAX_EXE_SYMLINK_PATH_LEN 30  // max possible size of path to exe symlink + 1 null byte

static void initApplicationFilePath()
{
#if defined(BMCL_PLATFORM_WINDOWS)
    wchar_t* buf = 0;
    DWORD bufSize = MAX_PATH;
    DWORD size = 0;
    DWORD i = 0;
    do {
        i++;
        bufSize = MAX_PATH * i + 1;
        buf = (wchar_t*)std::realloc(buf, bufSize * sizeof(wchar_t));
        if (!buf) {
            return;
        }
        size = GetModuleFileNameW(0, buf, bufSize);
    } while (size >= bufSize);
    buf[size] = L'\0';
    auto initPath = [](const wchar_t* src, DWORD size, Option<std::string>* dest) {
        int sizeNeeded = WideCharToMultiByte(CP_ACP, 0, src, (int)size, NULL, 0, NULL, NULL);
        dest->emplace(sizeNeeded, '\0');
        WideCharToMultiByte(CP_ACP, 0, src, (int)size, &dest->unwrap()[0], sizeNeeded, NULL, NULL);
    };
    initPath(buf, size, &appPath);
    PathRemoveFileSpecW(buf);
    initPath(buf, size, &appDirPath);
    std::free(buf);
#elif defined(BMCL_PLATFORM_UNIX)
    char path[PATH_MAX + 1];
    char exeSymlinkPath[MAX_EXE_SYMLINK_PATH_LEN];
    int exeSymlinkPathLen = std::sprintf(exeSymlinkPath, "/proc/%" PRIu64 "/exe", applicationPid());
    if (exeSymlinkPathLen < 0) {
        return;
    }
    BMCL_ASSERT(exeSymlinkPathLen < MAX_EXE_SYMLINK_PATH_LEN);
    exeSymlinkPath[exeSymlinkPathLen] = '\0';
    ssize_t pathLen = readlink(exeSymlinkPath, path, PATH_MAX);
    if (pathLen == -1) {
        return;
    }
    path[pathLen] = '\0';
    appPath.emplace(path);
    char* dirPath = dirname(path);
    appDirPath.emplace(dirPath);
#elif defined(BMCL_PLATFORM_APPLE)
    char path[PATH_MAX + 1];
    unsigned int len = PATH_MAX;
    if(_NSGetExecutablePath(path, &len) < 0)
        return;
    char* realPath = realpath(path, 0);
    if(realPath == NULL)
        return;
    appPath.emplace(realPath);
    free(realPath);
    char* dirPath = dirname(path);
    appDirPath.emplace(dirPath);
#endif
}

const Option<std::string>& applicationFilePath()
{
    std::call_once(appPathInitialized, initApplicationFilePath);
    return appPath;
}

const Option<std::string>& applicationDirPath()
{
    std::call_once(appPathInitialized, initApplicationFilePath);
    return appDirPath;
}
}

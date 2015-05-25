/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/Config.h"

#if BMCL_HAVE_QT
#include <QTextStream>
#include <QBuffer>
#else
#include <sstream>
#endif

#define BMCL_LOG(level) bmcl::Logger(level)
#define BMCL_DEBUG() BMCL_LOG(bmcl::LogLevel::Debug)
#define BMCL_INFO() BMCL_LOG(bmcl::LogLevel::Info)
#define BMCL_WARNING() BMCL_LOG(bmcl::LogLevel::Warning)
#define BMCL_CRITICAL() BMCL_LOG(bmcl::LogLevel::Critical)

namespace bmcl {

enum class LogLevel {
    None = 0, //
    Panic = 1,
    Critical = 2,
    Warning = 3,
    Info = 4,
    Debug = 5
};

typedef void (*LogHandler)(LogLevel level, const char* msg);

void setLogLevel(LogLevel level);
LogLevel logLevel();
void setLogHandler(LogHandler handler);
void setDefaulLogHandler();
void log(LogLevel level, const char* msg);

class Logger {
public:
    Logger(LogLevel level);
    ~Logger();

    template <typename T>
    Logger& operator<<(const T& value);

private:
    LogLevel _allowedLevel;
    LogLevel _level;
#if BMCL_HAVE_QT
    QTextStream _stream;
    QString _buffer;
#else
    std::ostringstream _stream;
#endif
};

inline Logger::Logger(LogLevel level)
    : _allowedLevel(bmcl::logLevel())
    , _level(level)
#if BMCL_HAVE_QT
    , _stream(&_buffer, QIODevice::WriteOnly)
#endif
{
}

template <typename T>
Logger& Logger::operator<<(const T& value)
{
    if ((int)_level <= (int)_allowedLevel) {
        _stream << value;
    }
    return *this;
}
};

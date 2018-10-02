/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "bmcl/Config.h"
#include "bmcl/Logging.h"
#include "bmcl/ColorStream.h"

#include <iostream>
#include <cstring>
#include <ctime>

namespace bmcl {

static LogLevel _currentLogLevel = LogLevel::Debug;

void setLogLevel(LogLevel level)
{
    _currentLogLevel = level;
}

LogLevel logLevel()
{
    return _currentLogLevel;
}

static void coutHandler(LogLevel level, const char* msg)
{
    if ((int)level > (int)_currentLogLevel) {
        return;
    }
    if (level == LogLevel::None) {
        return;
    }
    const char* prefix;
    ColorAttr attr;
    switch (level) {
    case LogLevel::Debug:
        prefix = "DEBUG:   ";
        attr = ColorAttr::FgMagenta;
        break;
    case LogLevel::Info:
        prefix = "INFO:    ";
        attr = ColorAttr::FgCyan;
        break;
    case LogLevel::Warning:
        prefix = "WARNING: ";
        attr = ColorAttr::FgYellow;
        break;
    case LogLevel::Critical:
        prefix = "CRITICAL:";
        attr = ColorAttr::FgRed;
        break;
    case LogLevel::Panic:
        prefix = "PANIC:   ";
        attr = ColorAttr::FgRed;
        break;
    default:
        prefix = "???: ";
    }
    char timeStr[20];
    std::time_t t = std::time(nullptr);
    std::strftime(timeStr, sizeof(timeStr), "%F %T", std::localtime(&t));
    ColorStdError out;
    out << ColorAttr::Bright << timeStr << ' ';
    out << attr << prefix << ColorAttr::Reset << ' ';
#ifdef BMCL_HAVE_QT
    out << QString::fromUtf8(msg).toLocal8Bit().constData() << std::endl;
#else
    out << msg << std::endl;
#endif
}

static LogHandler currentLogHandler = coutHandler;

void setLogHandler(const LogHandler& handler)
{
    currentLogHandler = handler;
}

void setLogHandler(LogHandler&& handler)
{
    currentLogHandler = std::move(handler);
}

void setDefaulLogHandler()
{
    currentLogHandler = coutHandler;
}

void log(LogLevel level, const char* msg)
{
    currentLogHandler(level, msg);
}

Logger::~Logger()
{
#ifdef BMCL_HAVE_QT
    log(_level, _buffer.toUtf8().constData());
#else
    log(_level, _stream.str().c_str());
#endif
}

Logger& Logger::operator<<(const char* msg)
{
    if ((int)_level <= (int)_currentLogLevel) {
#ifdef BMCL_HAVE_QT
        _stream << QString::fromUtf8(msg);
#else
        _stream << msg;
#endif
    }
    return *this;
}

template<>
Logger& Logger::operator<<(const std::string& value)
{
    if ((int)_level <= (int)_currentLogLevel) {
#ifdef BMCL_HAVE_QT
        _stream << QString::fromStdString(value);
#else
        _stream << value.c_str();
#endif
    }
    return *this;
}

}

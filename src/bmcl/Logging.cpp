#include "bmcl/Logging.h"

#include <iostream>
#include <cstring>

namespace bmcl {

static LogLevel currentLogLevel = LogLevel::Debug;

void setLogLevel(LogLevel level)
{
    currentLogLevel = level;
}

LogLevel logLevel()
{
    return currentLogLevel;
}

static void coutHandler(LogLevel level, const char* msg)
{
    if (level == LogLevel::None) {
        return;
    }
    const char* prefix;
    switch (level) {
    case LogLevel::Debug:
        prefix = "DEBUG: ";
        break;
    case LogLevel::Warning:
        prefix = "WARNING: ";
        break;
    case LogLevel::Critical:
        prefix = "CRITICAL: ";
        break;
    case LogLevel::Panic:
        prefix = "PANIC: ";
        break;
    default:
        prefix = "???: ";
    }
    std::cout << prefix << msg << std::endl;
}

static LogHandler currentLogHandler = coutHandler;

void setLogHandler(LogHandler handler)
{
    currentLogHandler = handler;
}

void log(LogLevel level, const char* msg)
{
    currentLogHandler(level, msg);
}

Logger::~Logger()
{
#if BMCL_HAVE_QT
    log(currentLogLevel, _buffer.toLocal8Bit().constData());
#else
    log(currentLogLevel, _stream.str().c_str());
#endif
}
}

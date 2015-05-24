#pragma once

#include <sstream>
#include <iostream>

#define BMCL_LOG(level) bmcl::Logger(level)
#define BMCL_DEBUG(level) BMCL_LOG(bmcl::LogLevel::Debug)
#define BMCL_WARNING(level) BMCL_LOG(bmcl::LogLevel::Warning)
#define BMCL_CRITICAL(level) BMCL_LOG(bmcl::LogLevel::Critical)

namespace bmcl {

enum class LogLevel {
    None = 0, //
    Panic = 1,
    Critical = 2,
    Warning = 3,
    Debug = 4
};

typedef void (*LogHandler)(LogLevel level, const char* msg);

void setLogLevel(LogLevel level);
LogLevel logLevel();
void setLogHandler(LogHandler handler);
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
    std::ostringstream _stream;
};

inline Logger::Logger(LogLevel level)
    : _allowedLevel(bmcl::logLevel())
    , _level(level)
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

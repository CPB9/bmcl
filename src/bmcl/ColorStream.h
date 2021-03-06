/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/Config.h"

#include <iostream>

namespace bmcl {

enum class ColorAttr {
    Reset = 0,

    Normal = 22,
    Bright = 1,

    FgBlack = 30,
    FgRed = 31,
    FgGreen = 32,
    FgYellow = 33,
    FgBlue = 34,
    FgMagenta = 35,
    FgCyan = 36,
    FgWhite = 37,

    BgBlack = 40,
    BgRed = 41,
    BgGreen = 42,
    BgYellow = 43,
    BgBlue = 44,
    BgMagenta = 45,
    BgCyan = 46,
    BgWhite = 47,
};

struct ColorStreamPrivate;

class BMCL_EXPORT ColorStream {
public:
    ~ColorStream();

    inline ColorStream& operator<<(std::ostream& (*func)(std::ostream&));

    template <typename T>
    ColorStream& operator<<(const T& value);

    inline void reset();

protected:
#ifdef BMCL_PLATFORM_WINDOWS
	enum HandleType {
		StdOut,
		StdErr
	};
    ColorStream(std::ostream* stream, HandleType handle);
#else
    ColorStream(std::ostream* stream);
#endif

private:
    void setAttribute(ColorAttr attr);

#ifdef BMCL_PLATFORM_WINDOWS
    void applyAttrs(ColorAttr colorAttr);
    void resetAttrs();
	ColorStreamPrivate* _d;
#endif
    std::ostream* _stream;
};

inline void ColorStream::reset()
{
    setAttribute(ColorAttr::Reset);
}

template <typename T>
inline ColorStream& ColorStream::operator<<(const T& value)
{
    *_stream << value;
    return *this;
}

inline ColorStream& ColorStream::operator<<(std::ostream& (*func)(std::ostream&))
{
    *_stream << func;
    return *this;
}

template <>
inline ColorStream& ColorStream::operator<<(const ColorAttr& attr)
{
    setAttribute(attr);
    return *this;
}

class BMCL_EXPORT ColorStdOut : public ColorStream {
public:
#ifdef BMCL_PLATFORM_WINDOWS
    inline ColorStdOut(): ColorStream(&std::cout, ColorStream::StdOut) {}
#else
    inline ColorStdOut(): ColorStream(&std::cout) {}
#endif
};

class BMCL_EXPORT ColorStdError : public ColorStream {
public:
#ifdef BMCL_PLATFORM_WINDOWS
    inline ColorStdError(): ColorStream(&std::cerr, ColorStream::StdErr) {}
#else
    inline ColorStdError(): ColorStream(&std::cerr) {}
#endif
};
}

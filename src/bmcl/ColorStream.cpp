/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "bmcl/Config.h"
#include "bmcl/ColorStream.h"

namespace bmcl {

#ifdef BMCL_PLATFORM_WINDOWS
ColorStream::ColorStream(std::ostream* stream, HANDLE handle)
    : _stream(stream)
    , _handle(handle)
#else
ColorStream::ColorStream(std::ostream* stream)
    : _stream(stream)
#endif
{
#ifdef BMCL_PLATFORM_WINDOWS
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(_handle, &info);
    _defaultAttrs = consoleInfo.wAttributes;
    _currentAttrs = _defaultAttrs;
#endif
}

#ifdef BMCL_PLATFORM_WINDOWS

static WORD applyAttrs(WORD attrs, WORD defaultAttrs, ColorAttr colorAttr)
{
    WORD noFgMask = ~(FOREGROUND_BLUE & FOREGROUND_GREEN & FOREGROUND_RED);
    WORD noBgMask = ~(BACKGROUND_BLUE & BACKGROUND_GREEN & BACKGROUND_RED);
    switch (colorAttr) {
    case ColorAttr::Reset:
        return defaultAttrs;
    case ColorAttr::Normal:
        return attrs | FOREGROUND_INTENSITY;
    case ColorAttr::Bright:
        return attrs & (~FOREGROUND_INTENSITY);
    case ColorAttr::FgBlack:
        return attrs & noFgMask;
    case ColorAttr::FgRed:
        return attrs & (noFgMask | FOREGROUND_RED);
    case ColorAttr::FgGreen:
        return attrs & (noFgMask | FOREGROUND_GREEN);
    case ColorAttr::FgYellow:
        return attrs & (noFgMask | FOREGROUND_GREEN | FOREGROUND_RED);
    case ColorAttr::FgBlue:
        return attrs & (noFgMask | FOREGROUND_BLUE);
    case ColorAttr::FgMagenta:
        return attrs & (noFgMask | FOREGROUND_BLUE | FOREGROUND_RED);
    case ColorAttr::FgCyan:
        return attrs & (noFgMask | FOREGROUND_BLUE | FOREGROUND_GREEN);
    case ColorAttr::FgWhite:
        return attrs | (~noFgMask);
    case ColorAttr::BgBlack:
        return attrs & noBgMask;
    case ColorAttr::BgRed:
        return attrs & (noBgMask | BACKEGROUND_RED);
    case ColorAttr::BgGreen:
        return attrs & (noBgMask | BACKEGROUND_GREEN);
    case ColorAttr::BgYellow:
        return attrs & (noBgMask | BACKEGROUND_GREEN | BACKEGROUND_RED);
    case ColorAttr::BgBlue:
        return attrs & (noBgMask | BACKEGROUND_BLUE);
    case ColorAttr::BgMagenta:
        return attrs & (noBgMask | BACKEGROUND_BLUE | BACKEGROUND_RED);
    case ColorAttr::BgCyan:
        return attrs & (noBgMask | BACKEGROUND_BLUE | BACKEGROUND_GREEN);
    case ColorAttr::BgWhite:
        return attrs | (~noBgMask);
    }
    return attrs;
}

void ColorStream::setAttribute(ColorAttr colorAttr)
{
    _currentAttrs = applyAttrs(_currentAttrs, _defaultAttrs, colorAttr);
    SetConsoleTextAttribute(handle, _currentAttrs);
}

#else

void ColorStream::setAttribute(ColorAttr colorAttr)
{
    *_stream << "\x1b[" << (int)colorAttr << 'm';
}

#endif

}

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
    _defaultAttrs = info.wAttributes;
    resetAttrs();
#endif
}

#ifdef BMCL_PLATFORM_WINDOWS

void ColorStream::resetAttrs()
{
    WORD fgMask = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
    _currentFg = _defaultAttrs & fgMask;
    WORD bgMask = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
    _currentBg = _defaultAttrs & bgMask;
    _currentIntensity = _defaultAttrs & FOREGROUND_INTENSITY; 
    _otherAttrs = _defaultAttrs & ~(fgMask | bgMask | FOREGROUND_INTENSITY);
}

void ColorStream::applyAttrs(ColorAttr colorAttr)
{
    switch (colorAttr) {
    case ColorAttr::Reset:
        SetConsoleTextAttribute(_handle, _defaultAttrs);
        resetAttrs();
        break;
    case ColorAttr::Normal:
        _currentIntensity = 0;
        break;
    case ColorAttr::Bright:
        _currentIntensity |= FOREGROUND_INTENSITY;
        break;
    case ColorAttr::FgBlack:
        _currentFg = 0;
        break;
    case ColorAttr::FgRed:
        _currentFg = FOREGROUND_RED;
        break;
    case ColorAttr::FgGreen:
        _currentFg = FOREGROUND_GREEN;
        break;
    case ColorAttr::FgYellow:
        _currentFg = FOREGROUND_GREEN | FOREGROUND_RED;
        break;
    case ColorAttr::FgBlue:
        _currentFg = FOREGROUND_BLUE;
        break;
    case ColorAttr::FgMagenta:
        _currentFg = FOREGROUND_BLUE | FOREGROUND_RED;
        break;
    case ColorAttr::FgCyan:
        _currentFg = FOREGROUND_BLUE | FOREGROUND_GREEN;
        break;
    case ColorAttr::FgWhite:
        _currentFg = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
        break;
    case ColorAttr::BgBlack:
        _currentBg = 0;
        break;
    case ColorAttr::BgRed:
        _currentBg = BACKGROUND_RED;
        break;
    case ColorAttr::BgGreen:
        _currentBg = BACKGROUND_GREEN;
        break;
    case ColorAttr::BgYellow:
        _currentBg = BACKGROUND_GREEN | BACKGROUND_RED;
        break;
    case ColorAttr::BgBlue:
        _currentBg = BACKGROUND_BLUE;
        break;
    case ColorAttr::BgMagenta:
        _currentBg = BACKGROUND_BLUE | BACKGROUND_RED;
        break;
    case ColorAttr::BgCyan:
        _currentBg = BACKGROUND_BLUE | BACKGROUND_GREEN;
        break;
    case ColorAttr::BgWhite:
        _currentBg = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
        break;
    }
}

void ColorStream::setAttribute(ColorAttr colorAttr)
{
    applyAttrs(colorAttr);
    SetConsoleTextAttribute(_handle, _currentFg | _currentBg | _currentIntensity | _otherAttrs);
}

#else

void ColorStream::setAttribute(ColorAttr colorAttr)
{
    *_stream << "\x1b[" << (int)colorAttr << 'm';
}

#endif

}

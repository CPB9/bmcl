/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "bmcl/Config.h"
#include "bmcl/ColorStream.h"

#ifdef BMCL_PLATFORM_WINDOWS
#include <stdio.h>
#include <windows.h>
#endif

namespace bmcl {

#ifdef BMCL_PLATFORM_WINDOWS

struct ColorStreamPrivate {
    WORD defaultAttrs;
    WORD currentFg;
    WORD currentBg;
    WORD currentIntensity;
    WORD otherAttrs;
    HANDLE handle;
};

ColorStream::ColorStream(std::ostream* stream, ColorStream::HandleType handle)
    : _stream(stream)
#else
ColorStream::ColorStream(std::ostream* stream)
    : _stream(stream)
#endif
{
#ifdef BMCL_PLATFORM_WINDOWS
	_d = new ColorStreamPrivate;
	if (handle == ColorStream::StdErr) {
		_d->handle = GetStdHandle(STD_ERROR_HANDLE);
	} else {
		_d->handle = GetStdHandle(STD_OUTPUT_HANDLE);
	}
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(_d->handle, &info);
    _d->defaultAttrs = info.wAttributes;
    resetAttrs();
#endif
}

ColorStream::~ColorStream()
{
    reset();
#ifdef BMCL_PLATFORM_WINDOWS
	delete _d;
#endif
}

#ifdef BMCL_PLATFORM_WINDOWS

void ColorStream::resetAttrs()
{
    WORD fgMask = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
    _d->currentFg = _d->defaultAttrs & fgMask;
    WORD bgMask = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
    _d->currentBg = _d->defaultAttrs & bgMask;
    _d->currentIntensity = _d->defaultAttrs & FOREGROUND_INTENSITY; 
    _d->otherAttrs = _d->defaultAttrs & ~(fgMask | bgMask | FOREGROUND_INTENSITY);
}

void ColorStream::applyAttrs(ColorAttr colorAttr)
{
    switch (colorAttr) {
    case ColorAttr::Reset:
        SetConsoleTextAttribute(_d->handle, _d->defaultAttrs);
        resetAttrs();
        break;
    case ColorAttr::Normal:
        _d->currentIntensity = 0;
        break;
    case ColorAttr::Bright:
        _d->currentIntensity |= FOREGROUND_INTENSITY;
        break;
    case ColorAttr::FgBlack:
        _d->currentFg = 0;
        break;
    case ColorAttr::FgRed:
        _d->currentFg = FOREGROUND_RED;
        break;
    case ColorAttr::FgGreen:
        _d->currentFg = FOREGROUND_GREEN;
        break;
    case ColorAttr::FgYellow:
        _d->currentFg = FOREGROUND_GREEN | FOREGROUND_RED;
        break;
    case ColorAttr::FgBlue:
        _d->currentFg = FOREGROUND_BLUE;
        break;
    case ColorAttr::FgMagenta:
        _d->currentFg = FOREGROUND_BLUE | FOREGROUND_RED;
        break;
    case ColorAttr::FgCyan:
        _d->currentFg = FOREGROUND_BLUE | FOREGROUND_GREEN;
        break;
    case ColorAttr::FgWhite:
        _d->currentFg = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
        break;
    case ColorAttr::BgBlack:
        _d->currentBg = 0;
        break;
    case ColorAttr::BgRed:
        _d->currentBg = BACKGROUND_RED;
        break;
    case ColorAttr::BgGreen:
        _d->currentBg = BACKGROUND_GREEN;
        break;
    case ColorAttr::BgYellow:
        _d->currentBg = BACKGROUND_GREEN | BACKGROUND_RED;
        break;
    case ColorAttr::BgBlue:
        _d->currentBg = BACKGROUND_BLUE;
        break;
    case ColorAttr::BgMagenta:
        _d->currentBg = BACKGROUND_BLUE | BACKGROUND_RED;
        break;
    case ColorAttr::BgCyan:
        _d->currentBg = BACKGROUND_BLUE | BACKGROUND_GREEN;
        break;
    case ColorAttr::BgWhite:
        _d->currentBg = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
        break;
    }
}

void ColorStream::setAttribute(ColorAttr colorAttr)
{
    applyAttrs(colorAttr);
    SetConsoleTextAttribute(_d->handle, _d->currentFg | _d->currentBg | _d->currentIntensity | _d->otherAttrs);
}

#else

void ColorStream::setAttribute(ColorAttr colorAttr)
{
    *_stream << "\x1b[" << (int)colorAttr << 'm';
}

#endif

}

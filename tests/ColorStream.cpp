/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "bmcl/Config.h"
#include "bmcl/ColorStream.h"

#include <iostream>

using bmcl::ColorAttr;

struct StrAttr {
    const char* name;
    ColorAttr attr;
};

static StrAttr fgPairs[8] = {
    {"Black", ColorAttr::FgBlack},
    {"Red", ColorAttr::FgRed},
    {"Green", ColorAttr::FgGreen},
    {"Yellow", ColorAttr::FgYellow},
    {"Blue", ColorAttr::FgBlue},
    {"Magenta", ColorAttr::FgMagenta},
    {"Cyan", ColorAttr::FgCyan},
    {"White", ColorAttr::FgWhite},
};

static StrAttr bgPairs[8] = {
    {"Black", ColorAttr::BgBlack},
    {"Red", ColorAttr::BgRed},
    {"Green", ColorAttr::BgGreen},
    {"Yellow", ColorAttr::BgYellow},
    {"Blue", ColorAttr::BgBlue},
    {"Magenta", ColorAttr::BgMagenta},
    {"Cyan", ColorAttr::BgCyan},
    {"White", ColorAttr::BgWhite},
};

static void printTable(const char* prefix, bmcl::ColorStdOut& out)
{
    for (int i = 0; i < 8; i++) {
        out << fgPairs[i].attr;
        for (int j = 0; j < 8; j++) {
            out << bgPairs[j].attr;
            out << prefix << ' ' << fgPairs[i].name << " on " << bgPairs[j].name << std::endl;
        }
    }
}

int main()
{
    bmcl::ColorStdOut out;
    out << ColorAttr::Bright;
    printTable("Bright", out);
    out << ColorAttr::Normal;
    printTable("Normal", out);
}

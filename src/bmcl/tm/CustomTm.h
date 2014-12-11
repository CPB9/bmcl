/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/core/Writer.h"
#include "bmcl/tm/Frame.h"

#include <cstddef>

namespace bmcl {
namespace tm {

class CustomTelemetry {
public:
    CustomTelemetry(std::size_t frameNum, std::size_t varsPerFrame)
    {

        _frames = new bmcl::tm::Frame[frameNum];
        _frameNum = frameNum;
    }

    bmcl::tm::Frame* frameAt(std::size_t index)
    {
        assert(index < _frameNum);
        return _frames[index];
    }

private:
    Frame* _frames;
    std::size_t _frameNum;
};
}
}

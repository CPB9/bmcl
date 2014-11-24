/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/core/writer.h"
#include "bmcl/telem/frame.h"

#include <cstddef>

namespace bmcl {
namespace telem {

class CustomTelemetry {
public:
    CustomTelemetry(std::size_t frameNum, std::size_t varsPerFrame)
    {

        _frames = new bmcl::telem::Frame[frameNum];
        _frameNum = frameNum;
    }

    bmcl::telem::Frame* frameAt(std::size_t index)
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

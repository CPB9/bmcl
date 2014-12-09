/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/core/status.h"
#include "bmcl/core/writer.h"
#include "bmcl/telem/frame.h"

#include <cstddef>

namespace bmcl {
namespace telem {

class Serializer {
public:
    virtual bmcl::core::Status::Msg serialize(const bmcl::telem::Frame* frame, bmcl::core::Writer* dest)
    {
        if (dest->availableSize() < frame->totalSize()) {
            return bmcl::core::Status::BufferOverflow;
        }
        copyFrameUnsafe(frame, dest);
        return bmcl::core::Status::Success;
    }

protected:
    void copyFrameUnsafe(const bmcl::telem::Frame* frame, bmcl::core::Writer* dest)
    {
        Variable* vars = frame->vars();
        std::size_t varsNum = frame->count();
        for (std::size_t i = 0; i < varsNum; i++) {
            dest->write(vars[i].ptr, vars[i].size);
        }
    }
};
}
}

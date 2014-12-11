/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <cassert>
#include <cstddef>

namespace bmcl {
namespace tm {

struct Variable {
    Variable()
        : ptr(0)
        , size(0)
    {
    }
    const void* ptr;
    std::size_t size;
};

class Frame {
public:
    Frame(std::size_t maxSize)
        : _maxCount(maxSize)
    {
        _vars = new Variable[maxSize];
    }

    std::size_t maxCount() const { return _maxCount; }
    std::size_t count() const { return _count; }
    std::size_t totalSize() const { return _totalSize; }
    const Variable* vars() const { return _vars; }

    void append(const void* ptr, std::size_t size)
    {
        assert(_count < _maxCount);
        _vars[_count].ptr = ptr;
        _vars[_count].size = size;
        _totalSize += size;
        _count++;
    }

    Variable* popBack()
    {
        assert(_count != 0);
        Variable* var = _vars[_count + 1];
        _totalSize -= var->size;
        _count--;
        return var;
    }

private:
    Variable* _vars;
    std::size_t _count;
    std::size_t _maxCount;
    std::size_t _totalSize;
};
}
}

#include "bmcl/core/endian.h"
#include "bmcl/core/ringbuf.h"

#include <cassert>
#include <stdint.h>
#include <cstring>

#define min(a, b) (((a) > (b)) ? (b) : (a))

namespace bmcl {
namespace core {

RingBuf::RingBuf(void* data, std::size_t size)
{
    init(data, size);
#if BMCL_HAVE_MALLOC
    _hasAllocatedMem = false;
#endif
}

#if BMCL_HAVE_MALLOC

RingBuf::RingBuf(std::size_t size)
{
    uint8_t* data = new uint8_t[size];
    init(data, size);
    _hasAllocatedMem = true;
}

RingBuf::~RingBuf()
{
    if (_hasAllocatedMem) {
        delete[] _data;
    }
}

#endif

void RingBuf::clear()
{
    _writeOffset = 0;
    _readOffset = 0;
    _freeSpace = _size;
}

void RingBuf::erase(std::size_t size)
{
    assert(_size - _freeSpace >= size);
    _freeSpace += size;
    _readOffset += size;
    if (_readOffset >= _size) {
        _readOffset -= _size;
    }
}

std::size_t RingBuf::availableSize() const { return freeSpace(); }

void RingBuf::read(void* dest, std::size_t size)
{
    peek(dest, size, 0);
    erase(size);
}

void RingBuf::init(void* data, std::size_t size)
{
    assert(size > 0);
    _data = (uint8_t*)data;
    _size = size;
    _freeSpace = size;
    _readOffset = 0;
    _writeOffset = 0;
}

void RingBuf::extend(std::size_t size)
{
    _writeOffset += size;
    if (_writeOffset >= _size) {
        _writeOffset -= _size;
    }
    _freeSpace -= size;
}

void RingBuf::write(const void* data, std::size_t size)
{
    assert(size > 0);
    assert(size <= _size);
    if (_freeSpace < size) {
        erase(size - _freeSpace);
    }

    if (_readOffset > _writeOffset) { /* *********w---------------r************ */
        std::memcpy(_data + _writeOffset, data, size);
    } else { /* ----------r**************w---------- */
        std::size_t rightData = _size - _writeOffset;
        std::size_t firstChunkSize = min(size, rightData);
        std::memcpy(_data + _writeOffset, data, firstChunkSize);
        if (size > firstChunkSize) {
            std::size_t secondChunkSize = size - firstChunkSize;
            std::memcpy(_data, (const uint8_t*)data + firstChunkSize, secondChunkSize);
        }
    }
    extend(size);
}

void RingBuf::peek(void* dest, std::size_t size, std::size_t offset) const
{
    std::size_t readOffset = _readOffset + offset;
    if (readOffset >= _size) {
        readOffset -= _size;
    }
    assert(size > 0);
    assert(size + offset <= _size - _freeSpace);
    if (readOffset < _writeOffset) { /* ----------r**************w---------- */
        std::memcpy(dest, _data + readOffset, size);
    } else { /* *********w---------------r************ */
        std::size_t rightData = _size - readOffset;
        std::size_t firstChunkSize = min(size, rightData);
        std::memcpy(dest, _data + readOffset, firstChunkSize);
        if (size > firstChunkSize) {
            std::size_t secondChunkSize = size - firstChunkSize;
            std::memcpy((uint8_t*)dest + firstChunkSize, _data, secondChunkSize);
        }
    }
}
}
}

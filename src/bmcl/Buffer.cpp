#include "bmcl/Buffer.h"

#include <cstdlib>
#include <cstring>
#include <utility>

#define GROWTH_FACTOR 2

namespace bmcl {

Buffer::Buffer()
    : _ptr(0)
    , _size(0)
    , _capacity(0)
{
}

Buffer::Buffer(std::size_t size)
    : _size(size)
    , _capacity(size)
{
    _ptr = (uint8_t*)std::malloc(size);
}

void Buffer::copyFrom(const Buffer& other)
{
    if (other._ptr) {
        _size = other._size;
        _capacity = other._capacity;
        _ptr = (uint8_t*)std::malloc(_capacity);
        std::memcpy(_ptr, other._ptr, _size);
    } else {
        _ptr = 0;
        _size = 0;
        _capacity = 0;
    }
}

Buffer::Buffer(const Buffer& other)
{
    copyFrom(other);
}

Buffer& Buffer::operator=(const Buffer& other)
{
    dealloc();
    copyFrom(other);
    return *this;
}

void Buffer::moveFrom(Buffer&& other)
{
    _ptr = other._ptr;
    _size = other._size;
    _capacity = other._capacity;
    other._ptr = 0;
    other._size = 0;
    other._capacity = 0;
}

Buffer::Buffer(Buffer&& other)
{
    moveFrom(std::move(other));
}

Buffer& Buffer::operator=(Buffer&& other)
{
    dealloc();
    moveFrom(std::move(other));
    return *this;
}

void Buffer::dealloc()
{
    if (_ptr) {
        std::free(_ptr);
    }
}

Buffer::~Buffer()
{
    dealloc();
}

void Buffer::realloc(std::size_t capacity)
{
    if (_ptr) {
        if (capacity == 0) {
            std::free(_ptr);
            _ptr = 0;
        } else {
            _ptr = (uint8_t*)std::realloc(_ptr, capacity);
        }
    } else {
        _ptr = (uint8_t*)std::malloc(capacity);
    }
    _capacity = capacity;
}

void Buffer::reserve(std::size_t capacity)
{
    if (capacity > _capacity) {
        realloc(capacity);
    }
}

void Buffer::resize(std::size_t size)
{
    if (size > _capacity) {
        realloc(size);
    }
    _size = size;
}

void Buffer::resize(std::size_t size, uint8_t filler)
{
    if (size > _capacity) {
        realloc(size);
        std::memset(_ptr + _size, size - _size, filler);
    }
    _size = size;
}

void Buffer::shrink()
{
    realloc(_size);
}

void Buffer::writeImpl(const void* data, std::size_t size)
{
    if ((_capacity - _size) < size) {
        realloc(BMCL_MAX(_capacity + size, _size * 1.5));
    }
    std::memcpy(_ptr + _size, data, size);
    _size += size;
}
}

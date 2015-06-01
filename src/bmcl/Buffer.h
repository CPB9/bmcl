#pragma once

#include "bmcl/Config.h"
#include "bmcl/Writer.h"

#include <cstddef>
#include <cstring>

namespace bmcl {

class Buffer : public Writer<Buffer> {
public:
    Buffer();
    Buffer(std::size_t size);
    Buffer(const Buffer& other);
    Buffer(Buffer&& other);
    ~Buffer();

    std::size_t size() const;
    std::size_t capacity() const;
    bool isEmpty() const;
    uint8_t* start() const;
    uint8_t* end() const;

    void resize(std::size_t size);
    void resize(std::size_t size, uint8_t filler);
    void reserve(std::size_t size);
    void shrink();

    void writeImpl(const void* data, std::size_t size);
    std::size_t writableSizeImpl() const;

    Buffer& operator=(const Buffer& other);
    Buffer& operator=(Buffer&& other);

    uint8_t& operator[](std::size_t index);
    uint8_t operator[](std::size_t index) const;

private:
    void dealloc();
    void copyFrom(const Buffer& other);
    void moveFrom(Buffer&& other);
    void realloc(std::size_t size);

    uint8_t* _ptr;
    std::size_t _size;
    std::size_t _capacity;
};

inline std::size_t Buffer::size() const
{
    return _size;
}

inline std::size_t Buffer::capacity() const
{
    return _capacity;
}

inline uint8_t* Buffer::start() const
{
    return _ptr;
}

inline uint8_t* Buffer::end() const
{
    return _ptr + _size;
}

inline bool Buffer::isEmpty() const
{
    return _size == 0;
}

inline std::size_t Buffer::writableSizeImpl() const
{
    return _capacity - _size;
}

inline uint8_t& Buffer::operator[](std::size_t index)
{
    BMCL_ASSERT(index < _size);
    return _ptr[index];
}

inline uint8_t Buffer::operator[](std::size_t index) const
{
    BMCL_ASSERT(index < _size);
    return _ptr[index];
}
}

#include "bmcl/core/memwriter.h"

namespace bmcl {
namespace core {

MemWriter::MemWriter(void* dest, std::size_t maxSize)
{
    init(dest, maxSize);
#if BMCL_HAVE_MALLOC
    hasAllocatedMem = false;
#endif
}

#if BMCL_HAVE_MALLOC

MemWriter::MemWriter(std::size_t maxSize)
{
    uint8_t* dest = new uint8_t[maxSize];
    init(dest, maxSize);
    hasAllocatedMem = true;
}

MemWriter::~MemWriter()
{
    if (hasAllocatedMem) {
        delete[] _start;
    }
}

#endif

void MemWriter::advance(std::size_t size)
{
    assert(availableSize() >= size);
    _current += size;
}

void MemWriter::write(const void* data, std::size_t size)
{
    assert(availableSize() >= size);
    std::memcpy(_current, data, size);
    _current += size;
}

std::size_t MemWriter::availableSize() const { return _end - _current; }

void MemWriter::push(const void* src, std::size_t size) { write(src, size); }

void MemWriter::pop(void* dest, std::size_t size)
{
    assert(sizeUsed() >= size);
    _current -= size;
    std::memcpy(dest, _current, size);
}

void MemWriter::fill(uint8_t byte, std::size_t size)
{
    assert(availableSize() >= size);
    std::memset(_current, byte, size);
    _current += size;
}

void MemWriter::fillUp(uint8_t byte)
{
    std::size_t size = _end - _current;
    std::memset(_current, byte, size);
    _current += size;
}

void MemWriter::init(void* dest, std::size_t maxSize)
{
    _start = (uint8_t*)dest;
    _current = _start;
    _end = _start + maxSize;
}
}
}

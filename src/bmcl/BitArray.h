#pragma once

#include "bmcl/Config.h"

#include <cstddef>
#include <cstdint>
#include <cassert>

namespace bmcl {

template <std::size_t B>
class BitArray {
public:
    static constexpr std::size_t dataSizeInBytes();

    bool isSet(std::size_t pos) const;

    void set(std::size_t pos);
    void clear(std::size_t pos);
    void assign(std::size_t pos, bool value);

    void setAll();
    void clearAll();

private:
    static constexpr std::size_t bitSizeToByteSize(std::size_t bits);

    std::uint8_t _data[dataSizeInBytes()] = {0};
};

template <std::size_t B>
constexpr std::size_t BitArray<B>::bitSizeToByteSize(std::size_t bits)
{
    return (bits % 8) ? (bits / 8) + 1 : (bits / 8);
}

template <std::size_t B>
constexpr std::size_t BitArray<B>::dataSizeInBytes()
{
    return bitSizeToByteSize(B);
}

template <std::size_t B>
bool BitArray<B>::isSet(std::size_t pos) const
{
    assert(pos < B);
    return _data[pos / 8] & (1 << (pos % 8));
}

template <std::size_t B>
void BitArray<B>::set(std::size_t pos)
{
    assert(pos < B);
    _data[pos / 8] |= (1u << (pos % 8));
}

template <std::size_t B>
void BitArray<B>::clear(std::size_t pos)
{
    assert(pos < B);
    _data[pos / 8] &= ~(1u << (pos % 8));
}

template <std::size_t B>
void BitArray<B>::assign(std::size_t pos, bool value)
{
    _data[pos / 8] ^= (-std::uint8_t(value) ^ _data[pos / 8]) & (1u << (pos % 8));
}

template <std::size_t B>
void BitArray<B>::setAll()
{
    for (std::size_t i = 0; i < dataSizeInBytes(); i++) {
        _data[i] = 0xff;
    }
}

template <std::size_t B>
void BitArray<B>::clearAll()
{
    for (std::size_t i = 0; i < dataSizeInBytes(); i++) {
        _data[i] = 0x00;
    }
}
}

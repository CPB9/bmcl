#include "bmcl/IpAddress.h"
#include "bmcl/Endian.h"

#include <cstring>

namespace bmcl {

Ipv4Address::Ipv4Address(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
#ifdef BMCL_LITTLE_ENDIAN
    : _data(uint32_t(a) | (uint32_t(b) << 8) | (uint32_t(c) << 16) | (uint32_t(d) << 24))
#else
    : _data((uint32_t(a) << 24) | (uint32_t(b) << 16) | (uint32_t(c) << 8) | uint32_t(d))
#endif
{
}

Ipv4Address::Ipv4Address(uint32_t address)
#ifdef BMCL_LITTLE_ENDIAN
    : _data(htobe32(address))
#else
    : _data(data)
#endif
{
}

uint32_t Ipv4Address::toUint32() const
{
#ifdef BMCL_LITTLE_ENDIAN
    return htobe32(_data);
#else
    return _data;
#endif
}

std::array<uint8_t, 4> Ipv4Address::toArray() const
{
    std::array<uint8_t, 4> data;
    std::memcpy(data.data(), &_data, 4);
    return data;
}

SocketAddressV4& SocketAddressV4::operator=(const SocketAddressV4& other)
{
    _address = other._address;
    _port = other._port;
    return *this;
}

SocketAddressV4& SocketAddressV4::operator=(SocketAddressV4&& other)
{
    _address = other._address;
    _port = other._port;
    return *this;
}

bool SocketAddressV4::operator==(const SocketAddressV4& other)
{
    return (_address == other._address) && (_port == other._port);
}
}

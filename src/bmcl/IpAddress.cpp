#include "bmcl/IpAddress.h"
#include "bmcl/Endian.h"

#include <cstring>

//TODO: move to endian.h

#ifdef BMCL_LITTLE_ENDIAN
# define hton32(value) htobe32(value)
# define ntoh32(value) be32toh(value)
# define hton16(value) htobe16(value)
# define ntoh16(value) be16toh(value)
#else
# define hton32(value) (value)
# define ntoh32(value) (value)
# define hton16(value) (value)
# define ntoh16(value) (value)
#endif

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
    : _data(hton32(address))
{
}

Ipv4Address::Ipv4Address(const std::array<uint8_t, 4>& address)
{
    std::memcpy(&_data, address.data(), 4);
}

uint32_t Ipv4Address::toUint32() const
{
    return ntoh32(_data);
}

std::array<uint8_t, 4> Ipv4Address::toArray() const
{
    std::array<uint8_t, 4> address;
    std::memcpy(address.data(), &_data, 4);
    return address;
}

SocketAddressV4::SocketAddressV4(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint16_t port)
    : _address(a, b, c, d)
    , _port(hton16(port))
{
}

SocketAddressV4::SocketAddressV4(uint32_t address, uint16_t port)
    : _address(address)
    , _port(hton16(port))
{
}

SocketAddressV4::SocketAddressV4(Ipv4Address address, uint16_t port)
    : _address(address)
    , _port(hton16(port))
{
}

SocketAddressV4::SocketAddressV4(const SocketAddressV4& other)
    : _address(other._address)
    , _port(other._port)
{
}

SocketAddressV4::SocketAddressV4(SocketAddressV4&& other)
    : _address(other._address)
    , _port(other._port)
{
}

uint16_t SocketAddressV4::port() const
{
    return ntoh16(_port);
}

void SocketAddressV4::setPort(uint16_t port)
{
    _port = hton16(port);
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

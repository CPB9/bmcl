#pragma once

#include "bmcl/Config.h"

#include <cstdint>
#include <array>

namespace bmcl {

class BMCL_EXPORT Ipv4Address {
public:
    Ipv4Address();
    Ipv4Address(uint8_t a, uint8_t b, uint8_t c, uint8_t d);
    Ipv4Address(uint32_t data);
    Ipv4Address(const std::array<uint8_t, 4>& address);
    Ipv4Address(const Ipv4Address& other);
    Ipv4Address(Ipv4Address&& other);

    uint32_t toUint32() const;
    std::array<uint8_t, 4> toArray() const;

    Ipv4Address& operator=(const Ipv4Address& other);
    Ipv4Address& operator=(Ipv4Address&& other);

    bool operator==(const Ipv4Address& other) const;

private:
    uint32_t _data;
};

inline Ipv4Address::Ipv4Address()
    : _data(0)
{
}

inline Ipv4Address::Ipv4Address(const Ipv4Address& other)
    : _data(other._data)
{
}

inline Ipv4Address::Ipv4Address(Ipv4Address&& other)
    : _data(other._data)
{
}

inline Ipv4Address& Ipv4Address::operator=(const Ipv4Address& other)
{
    _data = other._data;
    return *this;
}

inline Ipv4Address& Ipv4Address::operator=(Ipv4Address&& other)
{
    _data = other._data;
    return *this;
}

inline bool Ipv4Address::operator==(const Ipv4Address& other) const
{
    return _data == other._data;
}

class BMCL_EXPORT SocketAddressV4 {
public:
    SocketAddressV4();
    SocketAddressV4(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint16_t port);
    SocketAddressV4(uint32_t address, uint16_t port);
    SocketAddressV4(Ipv4Address address, uint16_t port);
    SocketAddressV4(const SocketAddressV4& other);
    SocketAddressV4(SocketAddressV4&& other);

    Ipv4Address address() const;
    uint16_t port() const;

    void setIpAddress(Ipv4Address address);
    void setPort(uint16_t port);

    SocketAddressV4& operator=(const SocketAddressV4& other);
    SocketAddressV4& operator=(SocketAddressV4&& other);

    bool operator==(const SocketAddressV4& other);

private:
    Ipv4Address _address;
    uint16_t _port;
};

inline SocketAddressV4::SocketAddressV4()
    : _port(0)
{
}

inline Ipv4Address SocketAddressV4::address() const
{
    return _address;
}

inline void SocketAddressV4::setIpAddress(Ipv4Address address)
{
    _address = address;
}
}

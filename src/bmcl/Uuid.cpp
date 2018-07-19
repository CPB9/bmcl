/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "bmcl/Uuid.h"
#include "bmcl/StringView.h"
#include "bmcl/Result.h"

#ifdef BMCL_HAVE_QT
# include <QString>
# include <QByteArray>
# include <QLatin1String>
# include <QUuid>
#endif

#include <algorithm>

namespace bmcl {

#if defined(BMCL_PLATFORM_UNIX)
    #include <uuid/uuid.h>
#elif defined(BMCL_PLATFORM_WINDOWS)
    #include <Combaseapi.h>
#elif defined(BMCL_PLATFORM_APPLE)
    #include <CoreFoundation/CFUUID.h>
#else
    #error "Unsupported platform"
#endif

Uuid::Uuid(std::uint32_t d1, std::uint16_t d2, std::uint16_t d3, std::uint64_t d4)
{
    _data[0]  = (d1 >> 24) & 0xff;
    _data[1]  = (d1 >> 16) & 0xff;
    _data[2]  = (d1 >>  8) & 0xff;
    _data[3]  = (d1      ) & 0xff;
    _data[4]  = (d2 >>  8) & 0xff;
    _data[5]  = (d2      ) & 0xff;
    _data[6]  = (d3 >>  8) & 0xff;
    _data[7]  = (d3      ) & 0xff;
    _data[8]  = (d4 >> 56) & 0xff;
    _data[9]  = (d4 >> 48) & 0xff;
    _data[10] = (d4 >> 40) & 0xff;
    _data[11] = (d4 >> 32) & 0xff;
    _data[12] = (d4 >> 24) & 0xff;
    _data[13] = (d4 >> 16) & 0xff;
    _data[14] = (d4 >>  8) & 0xff;
    _data[15] = (d4      ) & 0xff;
}

Uuid Uuid::create()
{
    Uuid u;

#if defined(BMCL_PLATFORM_UNIX)
    uuid_generate(u._data.data());
#elif defined(BMCL_PLATFORM_WINDOWS)
    GUID data;
    CoCreateGuid(&data);

    u._data[0] = (data.Data1 >> 24) & 0xff;
    u._data[1] = (data.Data1 >> 16) & 0xff;
    u._data[2] = (data.Data1 >>  8) & 0xff;
    u._data[3] = (data.Data1      ) & 0xff;
    u._data[4] = (data.Data2 >>  8) & 0xff;
    u._data[5] = (data.Data2      ) & 0xff;
    u._data[6] = (data.Data3 >>  8) & 0xff;
    u._data[7] = (data.Data3      ) & 0xff;

    u._data[8]  = data.Data4[0];
    u._data[9]  = data.Data4[1];
    u._data[10] = data.Data4[2];
    u._data[11] = data.Data4[3];
    u._data[12] = data.Data4[4];
    u._data[13] = data.Data4[5];
    u._data[14] = data.Data4[6];
    u._data[15] = data.Data4[7];
#elif defined(BMCL_PLATFORM_APPLE)
    CFUUID native = CFUUIDCreate(NULL);
    CFUUIDBytes data = CFUUIDGetUUIDBytes(native);
    CFRelease(native);

    u._data[0]  = data.byte0;
    u._data[1]  = data.byte1;
    u._data[2]  = data.byte2;
    u._data[3]  = data.byte3;
    u._data[4]  = data.byte4;
    u._data[5]  = data.byte5;
    u._data[6]  = data.byte6;
    u._data[7]  = data.byte7;
    u._data[8]  = data.byte8;
    u._data[9]  = data.byte9;
    u._data[10] = data.byte10;
    u._data[11] = data.byte11;
    u._data[12] = data.byte12;
    u._data[13] = data.byte13;
    u._data[14] = data.byte14;
    u._data[15] = data.byte15;
#endif

    return u;
}


template <typename C, typename T>
Result<Uuid, void> Uuid::uuidFromString(T view)
{
    static const uint8_t charIndexes_BracesDashes[16]    = {1, 3, 5, 7, 10, 12, 15, 17, 20, 22, 25, 27, 29, 31, 33, 35};
    static const uint8_t charIndexes_bracesNoDahes[16]   = {1, 3, 5, 7,  9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31};
    static const uint8_t charIndexes_noBracesDahes[16]   = {0, 2, 4, 6,  9, 11, 14, 16, 19, 21, 24, 26, 28, 30, 32, 34};
    static const uint8_t charIndexes_noBracesNoDahes[16] = {0, 2, 4, 6,  8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30};
    static const uint8_t dashIndexes_bracesDashes[4]   = {9, 14, 19, 24};
    static const uint8_t dashIndexes_noBracesDashes[4] = {8, 13, 18, 23};

    Bytes chars;
    Bytes dashes;
    if (view.size() == 36) {
        chars = Bytes::fromStaticArray(charIndexes_noBracesDahes);
        dashes = Bytes::fromStaticArray(dashIndexes_noBracesDashes);
    } else if (view.size() == 38) {
        if (view[0] != '{' || view[37] != '}') {
            return Result<Uuid, void>();
        }
        chars = Bytes::fromStaticArray(charIndexes_BracesDashes);
        dashes = Bytes::fromStaticArray(dashIndexes_bracesDashes);
    } else if (view.size() == 32) {
        chars = Bytes::fromStaticArray(charIndexes_noBracesNoDahes);
    } else if (view.size() == 34) {
        if (view[0] != '{' || view[33] != '}') {
            return Result<Uuid, void>();
        }
        chars = Bytes::fromStaticArray(charIndexes_bracesNoDahes);
    } else {
        return Result<Uuid, void>();
    }
    assert(chars.size() == 16);

    Uuid u;
    for (uint8_t dashIndex : dashes) {
        if (view[dashIndex] != '-') {
            return Result<Uuid, void>();
        }
    }

    for (std::size_t i = 0; i < chars.size(); i++) {
        uint8_t charIndex = chars.begin()[i];
        int leftChar = C(view[charIndex]);
        if (leftChar >= '0' && leftChar <= '9') {
            leftChar = leftChar - '0';
        } else if (leftChar >= 'a' && leftChar <= 'f') {
            leftChar = leftChar - 'a' + 10;
        } else if (leftChar >= 'A' && leftChar <= 'F') {
            leftChar = leftChar - 'A' + 10;
        } else {
            return Result<Uuid, void>();
        }
        int rightChar = C(view[charIndex + 1]);
        if (rightChar >= '0' && rightChar <= '9') {
            rightChar = rightChar - '0';
        } else if (rightChar >= 'a' && rightChar <= 'f') {
            rightChar = rightChar - 'a' + 10;
        } else if (rightChar >= 'A' && rightChar <= 'F') {
            rightChar = rightChar - 'A' + 10;
        } else {
            return Result<Uuid, void>();
        }
        u._data[i] = (leftChar << 4) | rightChar;
    }

    return u;
}

#ifdef BMCL_HAVE_QT

struct QCharToIntConverter {
    QCharToIntConverter(QChar c)
        : data(c.unicode())
    {
    }

    operator int() const
    {
        return data;
    }

    ushort data;
};

Result<Uuid, void> Uuid::createFromString(const QString& str)
{
    return uuidFromString<QCharToIntConverter>(ArrayView<QChar>(str.data(), str.size()));
}

Result<Uuid, void> Uuid::createFromString(const QStringRef& str)
{
    return uuidFromString<QCharToIntConverter>(ArrayView<QChar>(str.data(), str.size()));
}

Result<Uuid, void> Uuid::createFromString(const QByteArray& str)
{
    return uuidFromString<int>(StringView(str.data(), str.size()));
}

Result<Uuid, void> Uuid::createFromString(const QLatin1String& str)
{
    return uuidFromString<int>(StringView(str.data(), str.size()));
}

#endif

Result<Uuid, void> Uuid::createFromString(bmcl::StringView view)
{
    return uuidFromString<int>(view);
}

static const char* hexChars = "0123456789abcdef";

template <typename T>
inline void appendHex(uint8_t n, T* dest)
{
    dest->push_back(hexChars[(n & 0xf0) >> 4]);
    dest->push_back(hexChars[n & 0x0f]);
}

template <typename T>
void uuidToString(const Uuid::Data& data, T* dest)
{
    dest->reserve(dest->size() + 16 * 2 + 4);
    appendHex(data[0], dest);
    appendHex(data[1], dest);
    appendHex(data[2], dest);
    appendHex(data[3], dest);
    dest->push_back('-');
    appendHex(data[4], dest);
    appendHex(data[5], dest);
    dest->push_back('-');
    appendHex(data[6], dest);
    appendHex(data[7], dest);
    dest->push_back('-');
    appendHex(data[8], dest);
    appendHex(data[9], dest);
    dest->push_back('-');
    appendHex(data[10], dest);
    appendHex(data[11], dest);
    appendHex(data[12], dest);
    appendHex(data[13], dest);
    appendHex(data[14], dest);
    appendHex(data[15], dest);
}

std::string Uuid::toStdString() const
{
    std::string dest;
    uuidToString(_data, &dest);
    return dest;
}

void Uuid::toStdString(std::string* dest) const
{
    uuidToString(_data, dest);
}

std::uint32_t Uuid::part1() const
{
    return (std::uint32_t(_data[0]) << 24) |
           (std::uint32_t(_data[1]) << 16) |
           (std::uint32_t(_data[2]) <<  8) |
           (std::uint32_t(_data[3])      );
}

std::uint16_t Uuid::part2() const
{
    return (std::uint16_t(_data[4]) << 8) |
           (std::uint16_t(_data[5])     );
}

std::uint16_t Uuid::part3() const
{
    return (std::uint16_t(_data[6]) << 8) |
           (std::uint16_t(_data[7])     );
}

std::uint64_t Uuid::part4() const
{
    return (std::uint64_t(_data[8])  << 56) |
           (std::uint64_t(_data[9])  << 48) |
           (std::uint64_t(_data[10]) << 40) |
           (std::uint64_t(_data[11]) << 32) |
           (std::uint64_t(_data[12]) << 24) |
           (std::uint64_t(_data[13]) << 16) |
           (std::uint64_t(_data[14]) <<  8) |
           (std::uint64_t(_data[15])      );
}

bool Uuid::isNil() const
{
    return std::all_of(_data.begin(), _data.end(), [](uint8_t b) {
        return b == 0;
    });
}

#ifdef BMCL_HAVE_QT

Uuid::Uuid(const QUuid& quuid)
{
#if QT_VERSION > 0x051000
    _data = createFromString(quuid.toByteArray(QUuid::WithoutBraces)).unwrap()._data;
#else
    QByteArray tmp = quuid.toByteArray();
    _data = createFromString(StringView(tmp.data() + 1, tmp.size() - 2)).unwrap()._data;
#endif
}

QString Uuid::toQString() const
{
    QString dest;
    uuidToString(_data, &dest);
    return dest;
}

void Uuid::toQString(QString* dest) const
{
    uuidToString(_data, dest);
}

QByteArray Uuid::toQByteArray() const
{
    QByteArray dest;
    uuidToString(_data, &dest);
    return dest;
}

void Uuid::toQByteArray(QByteArray* dest) const
{
    uuidToString(_data, dest);
}

QUuid Uuid::toQUuid() const
{
    return QUuid(
        (uint(_data[0]) << 24) | (uint(_data[1]) << 16) | (uint(_data[2]) << 8) | uint(_data[3]),
        (ushort(_data[4]) << 8) | ushort(_data[5]),
        (ushort(_data[6]) << 8) | ushort(_data[7]),
        _data[8],
        _data[9],
        _data[10],
        _data[11],
        _data[12],
        _data[13],
        _data[14],
        _data[15]
    );
}

#endif
}

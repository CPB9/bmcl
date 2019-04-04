/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/Config.h"

#include <array>
#include <string>
#include <cstdint>

#ifdef BMCL_HAVE_QT
class QString;
class QStringRef;
class QByteArray;
class QLatin1String;
class QUuid;
#endif

namespace bmcl {

class StringView;
template <typename T, typename R>
class Result;
class Uuid;

class BMCL_EXPORT UuidStringRepr {
public:
    static constexpr std::size_t reprSize = 1 + 16 * 2 + 4 + 1;

    UuidStringRepr(const Uuid& uuid);

    bmcl::StringView view() const;
    const char* data() const;
    const char* c_str() const;

    static constexpr std::size_t size();

private:
    std::array<char, reprSize + 1> _data;
};

inline const char* UuidStringRepr::data() const
{
    return _data.data();
}

inline const char* UuidStringRepr::c_str() const
{
    return _data.data();
}

constexpr std::size_t UuidStringRepr::size()
{
    return reprSize;
}

class BMCL_EXPORT Uuid {
public:
    using Data = std::array<std::uint8_t, 16>;

    Uuid(std::uint32_t d1, std::uint16_t d2, std::uint16_t d3, std::uint64_t d4);

#ifdef BMCL_HAVE_QT
    explicit Uuid(const QUuid& quuid);
#endif

    Uuid(const Uuid& other) = default;
    Uuid& operator=(const Uuid& other) = default;

    static Uuid create();
    static Uuid createNil();
    static Result<Uuid, void> createFromString(bmcl::StringView view);

#ifdef BMCL_HAVE_QT
    static Result<Uuid, void> createFromString(const QString& str);
    static Result<Uuid, void> createFromString(const QStringRef& str);
    static Result<Uuid, void> createFromString(const QByteArray& str);
    static Result<Uuid, void> createFromString(const QLatin1String& str);
#endif

    template <typename T>
    static Uuid createFromStringOrNil(const T& str);

    const Data& data() const;
    std::uint32_t part1() const;
    std::uint16_t part2() const;
    std::uint16_t part3() const;
    std::uint64_t part4() const;
    bool isNil() const;

    std::string toStdString() const;
    void toStdString(std::string* dest) const;
    UuidStringRepr toStringRepr() const;

#ifdef BMCL_HAVE_QT
    QString toQString() const;
    void toQString(QString* dest) const;
    QByteArray toQByteArray() const;
    void toQByteArray(QByteArray* dest) const;
    QUuid toQUuid() const;
#endif

    bool operator==(const Uuid& other) const;
    bool operator!=(const Uuid& other) const;
    bool operator<(const Uuid& other) const;
    bool operator<=(const Uuid& other) const;
    bool operator>(const Uuid& other) const;
    bool operator>=(const Uuid& other) const;

private:
    template <typename C, typename T>
    static Result<Uuid, void> uuidFromString(T view);

    Uuid() = default;

    Data _data;
};

inline const Uuid::Data& Uuid::data() const
{
    return _data;
}

inline Uuid Uuid::createNil()
{
    Uuid u;
    u._data.fill(0);
    return u;
}

inline UuidStringRepr Uuid::toStringRepr() const
{
    return UuidStringRepr(*this);
}

inline bool Uuid::operator==(const Uuid& other) const
{
    return _data == other._data;
}

inline bool Uuid::operator!=(const Uuid& other) const
{
    return _data != other._data;
}

inline bool Uuid::operator<(const Uuid& other) const
{
    return _data < other._data;
}

inline bool Uuid::operator<=(const Uuid& other) const
{
    return _data <= other._data;
}

inline bool Uuid::operator>(const Uuid& other) const
{
    return _data > other._data;
}

inline bool Uuid::operator>=(const Uuid& other) const
{
    return _data >= other._data;
}

template <typename T>
Uuid Uuid::createFromStringOrNil(const T& str)
{
    return createFromString(str).unwrapOr(Uuid::createNil());
}
}

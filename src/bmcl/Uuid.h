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

class BMCL_EXPORT Uuid {
public:
    using Data = std::array<std::uint8_t, 16>;

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

    const Data& data() const;

    std::string toStdString() const;
    void toStdString(std::string* dest) const;

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
}

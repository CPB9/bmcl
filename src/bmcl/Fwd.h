/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/Config.h"

#include <cstddef>
#include <cstdint>
#include <limits>

namespace bmcl {

template <class... T>
struct AlignedUnion;

template <typename T>
class ArrayView;

template <typename T, typename E>
class Either;

template <typename T, std::size_t S>
class FixedArrayView;

template <typename T, typename D, D defaultValue>
class DefaultOption;

template <typename T>
class Option;

template <typename T>
using OptionPtr = DefaultOption<T*, std::nullptr_t, nullptr>;

template <typename T>
class Rc;

template <typename T>
using OptionRc = DefaultOption<Rc<T>, std::nullptr_t, nullptr>;

using OptionSize = DefaultOption<std::size_t, std::size_t, static_cast<size_t>(-1)>;

template <typename B>
class Reader;

template <typename T, typename E>
class Result;

template <typename T>
class RefCountable;

template <std::size_t bits>
class Sha3;

class ThreadSafeRefCountableBase;

template <typename T>
using ThreadSafeRefCountable = ThreadSafeRefCountableBase;

template <typename B>
class Writer;

class Buffer;
class ColorStream;
class MemReader;
class MemWriter;
class RingBuffer;
class StringView;
class SharedBytes;
class Ipv4Address;
class SocketAddressV4;

using Bytes = ArrayView<std::uint8_t>;
}

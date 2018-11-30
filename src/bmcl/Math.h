/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/Config.h"

namespace bmcl {

#define BMCL_DEFINE_MATH_CONSTANT(name, value) \
template <typename T>                          \
constexpr inline T name();                     \
                                               \
template <>                                    \
constexpr inline float name()                  \
{                                              \
    return value##F;                           \
}                                              \
                                               \
template <>                                    \
constexpr inline double name()                 \
{                                              \
    return value;                              \
}                                              \
                                               \
template <>                                    \
constexpr inline long double name()            \
{                                              \
    return value##L;                           \
}                                              \

BMCL_DEFINE_MATH_CONSTANT(pi,       3.141592653589793238462643383279502884e+00)
BMCL_DEFINE_MATH_CONSTANT(halfPi,   1.570796326794896619231321691639751442e+00)
BMCL_DEFINE_MATH_CONSTANT(thirdPi,  1.047197551196597746154214461093167628e+00)
BMCL_DEFINE_MATH_CONSTANT(fourthPi, 7.853981633974483096156608458198757210e-01)
BMCL_DEFINE_MATH_CONSTANT(sixthPi,  5.235987755982988730771072305465838140e-01)
BMCL_DEFINE_MATH_CONSTANT(degree,   1.745329251994329576923690768488612713e-02)
BMCL_DEFINE_MATH_CONSTANT(radian,   5.729577951308232087679815481410517033e+01)

template <typename T>
constexpr inline T degreesToRadians(T degrees)
{
    return degrees * bmcl::degree<T>();
}

template <typename T>
constexpr inline T radiansToDegrees(T radians)
{
    return radians * bmcl::radian<T>();
}
}

#include "bmcl/Sha3.h"

#include "BmclTest.h"
#include "bmcl/Logging.h"
#include "bmcl/ArrayView.h"
#include "bmcl/StringView.h"

#include <array>
#include <cstddef>
#include <cstdio>
#include <cstring>

using namespace bmcl;

/* *************************** Self Tests ************************ */

/*
 * Known answer tests are from NIST SHA3 test vectors at
 * http://csrc.nist.gov/groups/ST/toolkit/examples.html
 *
 * SHA3-256:
 *   http://csrc.nist.gov/groups/ST/toolkit/documents/Examples/SHA3-256_Msg0.pdf
 *   http://csrc.nist.gov/groups/ST/toolkit/documents/Examples/SHA3-256_1600.pdf
 * SHA3-384:
 *   http://csrc.nist.gov/groups/ST/toolkit/documents/Examples/SHA3-384_1600.pdf
 * SHA3-512:
 *   http://csrc.nist.gov/groups/ST/toolkit/documents/Examples/SHA3-512_1600.pdf
 *
 * These are refered to as [FIPS 202] tests.
 */

template <typename T>
class Sha3Test : public ::testing::Test {
protected:

};

struct A3Data {
public:
    A3Data()
    {
        data.fill(0xa3);
    }

    std::array<uint8_t, 200> data;
};

struct Sha3_256Empty {
public:
    Sha3<256> ctx;
    Bytes data = Bytes::empty();
    Bytes hash = {{
        0xa7, 0xff, 0xc6, 0xf8, 0xbf, 0x1e, 0xd7, 0x66,
        0x51, 0xc1, 0x47, 0x56, 0xa0, 0x61, 0xd6, 0x62,
        0xf5, 0x80, 0xff, 0x4d, 0xe4, 0x3b, 0x49, 0xfa,
        0x82, 0xd8, 0x0a, 0x4b, 0x80, 0xf8, 0x43, 0x4a
    }};
};

struct Sha3_256Kessak {
public:
    Sha3<256> ctx;
    StringView data =
        "\xb7\x71\xd5\xce\xf5\xd1\xa4\x1a"
        "\x93\xd1\x56\x43\xd7\x18\x1d\x2a"
        "\x2e\xf0\xa8\xe8\x4d\x91\x81\x2f"
        "\x20\xed\x21\xf1\x47\xbe\xf7\x32"
        "\xbf\x3a\x60\xef\x40\x67\xc3\x73"
        "\x4b\x85\xbc\x8c\xd4\x71\x78\x0f"
        "\x10\xdc\x9e\x82\x91\xb5\x83\x39"
        "\xa6\x77\xb9\x60\x21\x8f\x71\xe7"
        "\x93\xf2\x79\x7a\xea\x34\x94\x06"
        "\x51\x28\x29\x06\x5d\x37\xbb\x55"
        "\xea\x79\x6f\xa4\xf5\x6f\xd8\x89"
        "\x6b\x49\xb2\xcd\x19\xb4\x32\x15"
        "\xad\x96\x7c\x71\x2b\x24\xe5\x03"
        "\x2d\x06\x52\x32\xe0\x2c\x12\x74"
        "\x09\xd2\xed\x41\x46\xb9\xd7\x5d"
        "\x76\x3d\x52\xdb\x98\xd9\x49\xd3"
        "\xb0\xfe\xd6\xa8\x05\x2f\xbb"
    ;
    StringView hash =
        "\xa1\x9e\xee\x92\xbb\x20\x97\xb6"
        "\x4e\x82\x3d\x59\x77\x98\xaa\x18"
        "\xbe\x9b\x7c\x73\x6b\x80\x59\xab"
        "\xfd\x67\x79\xac\x35\xac\x81\xb5"
    ;
};

struct Sha3_256A3 : public A3Data {
    Sha3<256> ctx;
    Bytes hash = {{
        0x79, 0xf3, 0x8a, 0xde, 0xc5, 0xc2, 0x03, 0x07,
        0xa9, 0x8e, 0xf7, 0x6e, 0x83, 0x24, 0xaf, 0xbf,
        0xd4, 0x6c, 0xfd, 0x81, 0xb2, 0x2e, 0x39, 0x73,
        0xc6, 0x5f, 0xa1, 0xbd, 0x9d, 0xe3, 0x17, 0x87
    }};
};

struct Sha3_384A3 : public A3Data {
    Sha3<384> ctx;
    Bytes hash = {{
        0x18, 0x81, 0xde, 0x2c, 0xa7, 0xe4, 0x1e, 0xf9,
        0x5d, 0xc4, 0x73, 0x2b, 0x8f, 0x5f, 0x00, 0x2b,
        0x18, 0x9c, 0xc1, 0xe4, 0x2b, 0x74, 0x16, 0x8e,
        0xd1, 0x73, 0x26, 0x49, 0xce, 0x1d, 0xbc, 0xdd,
        0x76, 0x19, 0x7a, 0x31, 0xfd, 0x55, 0xee, 0x98,
        0x9f, 0x2d, 0x70, 0x50, 0xdd, 0x47, 0x3e, 0x8f
    }};
};

struct Sha3_512A3 : public A3Data {
    Sha3<512> ctx;
    Bytes hash = {{
        0xe7, 0x6d, 0xfa, 0xd2, 0x20, 0x84, 0xa8, 0xb1,
        0x46, 0x7f, 0xcf, 0x2f, 0xfa, 0x58, 0x36, 0x1b,
        0xec, 0x76, 0x28, 0xed, 0xf5, 0xf3, 0xfd, 0xc0,
        0xe4, 0x80, 0x5d, 0xc4, 0x8c, 0xae, 0xec, 0xa8,
        0x1b, 0x7c, 0x13, 0xc3, 0x0a, 0xdf, 0x52, 0xa3,
        0x65, 0x95, 0x84, 0x73, 0x9a, 0x2d, 0xf4, 0x6b,
        0xe5, 0x89, 0xc5, 0x1c, 0xa1, 0xa4, 0xa8, 0x41,
        0x6d, 0xf6, 0x54, 0x5a, 0x1c, 0xe8, 0xba, 0x00
    }};
};

typedef ::testing::Types<Sha3_256Empty,
                         Sha3_256Kessak,
                         Sha3_256A3,
                         Sha3_384A3,
                         Sha3_512A3> MyTypes;

TYPED_TEST_CASE(Sha3Test, MyTypes);

TYPED_TEST(Sha3Test, OneStep)
{
    TypeParam param;
    param.ctx.update(param.data.data(), param.data.size());
    auto hash = param.ctx.finalize();
    EXPECT_EQ_MEM(param.hash.data(), hash.data(), param.hash.size());
}

TYPED_TEST(Sha3Test, TwoSteps)
{
    TypeParam param;
    std::size_t firstChunk = param.data.size() / 2;
    std::size_t secondChunk = param.data.size() - firstChunk;
    param.ctx.update(param.data.data(), firstChunk);
    param.ctx.update(param.data.data() + firstChunk, secondChunk);
    auto hash = param.ctx.finalize();
    EXPECT_EQ_MEM(param.hash.data(), hash.data(), param.hash.size());
}

TYPED_TEST(Sha3Test, ByteByByte)
{
    TypeParam param;
    for (std::size_t i = 0; i < param.data.size(); i++) {
        param.ctx.update(param.data.data() + i, 1);
    }
    auto hash = param.ctx.finalize();
    EXPECT_EQ_MEM(param.hash.data(), hash.data(), param.hash.size());
}

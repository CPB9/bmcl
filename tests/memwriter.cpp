#include "bmcl/endian.h"
#include "bmcl/memwriter.h"

#include <gtest/gtest.h>

class PackerTest: public ::testing::Test
{
protected:
    void SetUp()
    {
        memset(&_writer, 0, sizeof(_writer));
        memset(_data, 0, sizeof(_data));
    }

    void initWriter(size_t size)
    {
        assert(size <= 10000);
        memwriter_init(&_writer, _data, size);
    }

    memwriter_t* newPacker(size_t size)
    {
        memwriter_t* w = new memwriter_t;
        uint8_t* data = new uint8_t[size];
        memwriter_init(w, data, size);
        return w;
    }

    void expectData(void* expected, size_t size)
    {
        EXPECT_EQ(0, memcmp(memwriter_ptr(&_writer), expected, size));
    }

    void expectSizes(size_t used, size_t left)
    {

        EXPECT_EQ(used, memwriter_size(&_writer));
        EXPECT_EQ(left, memwriter_size_left(&_writer));
        EXPECT_EQ(used + left, memwriter_max_size(&_writer));
        bool isFull = used == memwriter_max_size(&_writer);
        EXPECT_EQ(isFull, memwriter_is_full(&_writer));
    }

    void append(const void* data, size_t size)
    {
        memwriter_write(&_writer, data, size);
    }

    void appendUint8(uint8_t data)
    {
        memwriter_write_uint8(&_writer, data);
    }

    void appendUint16le(uint16_t data)
    {
        memwriter_write_uint16le(&_writer, data);
    }

    void appendUint16be(uint16_t data)
    {
        memwriter_write_uint16be(&_writer, data);
    }

    void appendUint32be(uint32_t data)
    {
        memwriter_write_uint32be(&_writer, data);
    }

    void appendUint64be(uint64_t data)
    {
        memwriter_write_uint64be(&_writer, data);
    }

    void fillUp(int8_t data)
    {
        memwriter_fillup(&_writer, data);
    }

private:
    memwriter_t _writer;
    uint8_t _data[10000];
};

TEST_F(PackerTest, init)
{
    initWriter(100);
    expectSizes(0, 100);
}

TEST_F(PackerTest, append)
{
    uint8_t data1[4] = {0xaa, 0xbb, 0xcc, 0xdd};
    uint8_t data2[4] = {0x11, 0x22, 0x33, 0x44};
    uint8_t expected_data[8] = {0xaa, 0xbb, 0xcc, 0xdd, 0x11, 0x22, 0x33, 0x44};
    initWriter(8);
    append(data1, 4);
    append(data2, 4);
    expectData(expected_data, 8);
    expectSizes(8, 0);
}

TEST_F(PackerTest, fill_up)
{
    uint8_t expected[8] = {0xaa, 0xff, 0xcc, 0xdd, 0x11, 0x11, 0x11, 0x11};
    initWriter(8);
    appendUint8(0xaa);
    appendUint8(0xff);
    appendUint8(0xcc);
    appendUint8(0xdd);
    fillUp(0x11);
    expectData(expected, 8);
    expectSizes(8, 0);
}

TEST_F(PackerTest, append_uint8__one)
{
    uint8_t expected_data[] = {0xa1};
    initWriter(20);
    appendUint8(0xa1);
    expectData(expected_data, 1);
    expectSizes(1, 19);
}

TEST_F(PackerTest, append_uint8__several)
{
    uint8_t expected_data[] = {0xff, 0xaa, 0x12, 0x15};
    initWriter(100);
    appendUint8(0xff);
    appendUint8(0xaa);
    appendUint8(0x12);
    appendUint8(0x15);
    expectData(expected_data, 4);
    expectSizes(4, 96);
}

TEST_F(PackerTest, append_uint8__full)
{
    uint8_t expected_data[] = {0x11, 0x22, 0x33, 0x44, 0x55};
    initWriter(5);
    appendUint8(0x11);
    appendUint8(0x22);
    appendUint8(0x33);
    appendUint8(0x44);
    appendUint8(0x55);
    expectData(expected_data, 5);
    expectSizes(5, 0);
}

TEST_F(PackerTest, append_uint16__one)
{
#ifdef BMCL_LITTLE_ENDIAN
    uint16_t expected_data[] = {0x5847};
#else
    uint16_t expected_data[] = {0x4758};
#endif
    initWriter(20);
    appendUint16le(0x5847);
    expectData(expected_data, 2);
    expectSizes(2, 18);
}

TEST_F(PackerTest, append_uint16__full)
{
#ifdef BMCL_LITTLE_ENDIAN
    uint16_t expected_data[] = {0x1234, 0x7654, 0x0853, 0xcf41, 0xf017};
#else
    uint16_t expected_data[] = {0x3412, 0x5476, 0x5308, 0x41cf, 0x17f0};
#endif
    initWriter(10);
    appendUint16le(0x1234);
    appendUint16le(0x7654);
    appendUint16le(0x0853);
    appendUint16le(0xcf41);
    appendUint16le(0xf017);
    expectData(expected_data, 10);
    expectSizes(10, 0);
}

TEST_F(PackerTest, append_uint16be__one)
{
#ifdef BMCL_LITTLE_ENDIAN
    uint16_t expected_data[] = {0xaa55};
#else
    uint16_t expected_data[] = {0x55aa};
#endif
    initWriter(20);
    appendUint16be(0x55aa);
    expectData(expected_data, 2);
    expectSizes(2, 18);
}

TEST_F(PackerTest, append_uint16be__full)
{
#ifdef BMCL_LITTLE_ENDIAN
    uint16_t expected_data[] = {0xaa55, 0x1234, 0x5467, 0x0987, 0x10cc};
#else
    uint16_t expected_data[] = {0x55aa, 0x3412, 0x6754, 0x8709, 0xcc10};
#endif
    initWriter(10);
    appendUint16be(0x55aa);
    appendUint16be(0x3412);
    appendUint16be(0x6754);
    appendUint16be(0x8709);
    appendUint16be(0xcc10);
    expectData(expected_data, 10);
    expectSizes(10, 0);
}

TEST_F(PackerTest, append_uint32be__one)
{
#ifdef BMCL_LITTLE_ENDIAN
    uint32_t expected_data[] = {0x78563412};
#else
    uint32_t expected_data[] = {0x12345678};
#endif
    initWriter(20);
    appendUint32be(0x12345678);
    expectData(expected_data, 4);
    expectSizes(4, 16);
}

TEST_F(PackerTest, append_uint32be__full)
{
#ifdef BMCL_LITTLE_ENDIAN
    uint32_t expected_data[] = {0x78563412, 0xddccbbaa, 0x44332211, 0x21436587};
#else
    uint32_t expected_data[] = {0x12345678, 0xaabbccdd, 0x11223344, 0x87654321};
#endif
    initWriter(16);
    appendUint32be(0x12345678);
    appendUint32be(0xaabbccdd);
    appendUint32be(0x11223344);
    appendUint32be(0x87654321);
    expectData(expected_data, 16);
    expectSizes(16, 0);
}

TEST_F(PackerTest, append_uint64be__one)
{
#ifdef BMCL_LITTLE_ENDIAN
    uint64_t expected_data[] = {0xefcdab9078563412};
#else
    uint64_t expected_data[] = {0x1234567890abcdef};
#endif
    initWriter(20);
    appendUint64be(0x1234567890abcdef);
    expectData(expected_data, 8);
    expectSizes(8, 12);
}

TEST_F(PackerTest, append_uint64be__full)
{
#ifdef BMCL_LITTLE_ENDIAN
    uint64_t expected_data[] = {0x2222222211111111, 0x4444444433333333};
#else
    uint64_t expected_data[] = {0x1111111122222222, 0x3333333344444444};
#endif
    initWriter(16);
    appendUint64be(0x1111111122222222);
    appendUint64be(0x3333333344444444);
    expectData(expected_data, 16);
    expectSizes(16, 0);
}

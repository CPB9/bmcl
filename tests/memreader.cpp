#include "bmcl/core/memreader.h"

#include <gtest/gtest.h>

class ReaderTest: public ::testing::Test
{
protected:
    void SetUp()
    {
        _reader = 0;
    }

    void TearDown()
    {
        if (_reader) {
            memreader_destroy(_reader);
        }
    }

    void initReader(const void* data, size_t size)
    {
        assert(_reader == 0);
        _reader = memreader_create(data, size);
    }

    void expectParams(size_t read, size_t left, void* data)
    {

        EXPECT_EQ(read, memreader_size_read(_reader));
        EXPECT_EQ(left, memreader_size_left(_reader));
        EXPECT_EQ(read + left, memreader_size(_reader));
        bool isEmpty = read == memreader_size(_reader);
        EXPECT_EQ(isEmpty, memreader_is_empty(_reader));
        EXPECT_EQ((uint8_t*)data, memreader_current_ptr(_reader));
    }

    void skip(size_t size)
    {
        memreader_skip(_reader, size);
    }

    void peek(void* dest, size_t size, size_t offset)
    {
        memreader_peek(_reader, dest, size, offset);
    }

    uint8_t readUint8()
    {
        return memreader_read_uint8(_reader);
    }

    memreader_t* _reader;
};

TEST_F(ReaderTest, init)
{
    uint8_t data[2] = {0xff, 0xff};
    initReader(data, 2);
    expectParams(0, 2, data);
}

TEST_F(ReaderTest, current_ptr__after_read)
{
    uint8_t data[4] = {0xff, 0xff, 0xff, 0xff};
    initReader(data, 4);
    readUint8();
    expectParams(1, 3, data + 1);
}

TEST_F(ReaderTest, current_ptr__after_full_read)
{
    uint8_t data[4] = {0xff, 0xff, 0xff, 0xff};
    initReader(data, 4);
    readUint8();
    readUint8();
    readUint8();
    readUint8();
    expectParams(4, 0, data + 4);
}

TEST_F(ReaderTest, skip__zero)
{
    uint8_t data[4] = {0xff, 0xff, 0xff, 0xff};
    initReader(data, 4);
    skip(0);
    expectParams(0, 4, data);
}

TEST_F(ReaderTest, skip__several)
{
    uint8_t data[4] = {0xff, 0xff, 0xff, 0xff};
    initReader(data, 4);
    skip(2);
    expectParams(2, 2, data + 2);
}

TEST_F(ReaderTest, skip__all)
{
    uint8_t data[4] = {0xff, 0xff, 0xff, 0xff};
    initReader(data, 4);
    skip(4);
    expectParams(4, 0, data + 4);
}

TEST_F(ReaderTest, peek_one)
{
    uint8_t data[4] = {0xaa, 0xbb, 0xcc, 0xdd};
    uint8_t dest[4] = {0x00, 0x00, 0x00, 0x00};
    initReader(data, 4);
    peek(dest, 4, 0);
    expectParams(0, 4, data);
}

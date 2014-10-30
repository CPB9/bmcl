#include "bmcl/core/memreader.h"

#include "bmcl-test.h"

class MemReaderTest : public ::testing::Test {
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

    template <std::size_t n, typename R>
    void initReader(const R (&array)[n])
    {
        assert(_reader == 0);
        _reader = memreader_create(array, sizeof(R) * n);
    }

    void expectParams(std::size_t read, std::size_t left, void* data)
    {
        EXPECT_EQ(read, memreader_size_read(_reader));
        EXPECT_EQ(left, memreader_size_left(_reader));
        EXPECT_EQ(read + left, memreader_size(_reader));
        bool isEmpty = read == memreader_size(_reader);
        EXPECT_EQ(isEmpty, memreader_is_empty(_reader));
        EXPECT_EQ((uint8_t*)data, memreader_current_ptr(_reader));
    }

    void skip(std::size_t size)
    {
        memreader_skip(_reader, size);
    }

    void peek(void* dest, std::size_t size, std::size_t offset)
    {
        memreader_peek(_reader, dest, size, offset);
    }

    void read(void* dest, std::size_t size)
    {
        memreader_read(_reader, dest, size);
    }

    void expectNextUint8(uint8_t value)
    {
        EXPECT_EQ(value, memreader_read_uint8(_reader));
    }

    template <std::size_t n, typename R>
    void expectNextData(const R (&array)[n])
    {
        std::size_t dataSize = sizeof(R) * n;
        uint8_t temp[dataSize];
        memreader_read(_reader, temp, dataSize);
        EXPECT_EQ_MEM(array, temp, dataSize);
    }

    memreader_t* _reader;
};

TEST_F(MemReaderTest, init)
{
    uint8_t data[2] = {0xff, 0xff};
    initReader(data);
    expectParams(0, 2, data);
}

TEST_F(MemReaderTest, current_ptr_after_read)
{
    uint8_t data[4] = {0x22, 0xff, 0xff, 0xff};
    initReader(data);
    expectNextUint8(0x22);
    expectParams(1, 3, data + 1);
}

TEST_F(MemReaderTest, current_ptr_after_full_read)
{
    uint8_t data[4] = {0xff, 0xaa, 0x11, 0x55};
    initReader(data);
    expectNextUint8(0xff);
    expectNextUint8(0xaa);
    expectNextUint8(0x11);
    expectNextUint8(0x55);
    expectParams(4, 0, data + 4);
}

TEST_F(MemReaderTest, skip_zero)
{
    uint8_t data[4] = {0xff, 0xff, 0xff, 0xff};
    initReader(data);
    skip(0);
    expectParams(0, 4, data);
}

TEST_F(MemReaderTest, skip_several)
{
    uint8_t data[4] = {0xff, 0xff, 0xff, 0xff};
    initReader(data);
    skip(2);
    expectParams(2, 2, data + 2);
}

TEST_F(MemReaderTest, skip_all)
{
    uint8_t data[4] = {0xff, 0xff, 0xff, 0xff};
    initReader(data);
    skip(4);
    expectParams(4, 0, data + 4);
}

TEST_F(MemReaderTest, peek)
{
    uint8_t data[4] = {0xaa, 0xbb, 0xcc, 0xdd};
    uint8_t dest[4] = {0x00, 0x00, 0x00, 0x00};
    uint8_t expected[4] = {0xaa, 0xbb, 0xcc, 0xdd};
    initReader(data);
    peek(dest, 4, 0);
    expectParams(0, 4, data);
    EXPECT_EQ_ARRAYS(expected, dest);
}

TEST_F(MemReaderTest, read)
{
    uint8_t data[4] = {0xaa, 0xbb, 0xcc, 0xdd};
    uint8_t expected[4] = {0xaa, 0xbb, 0xcc, 0xdd};
    initReader(data);
    expectNextData(expected);
    expectParams(4, 0, data + 4);
}

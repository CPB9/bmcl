#include "bmcl/MemReader.h"

#include "BmclTest.h"
#include "bmcl/Logging.h"

using namespace bmcl;

class MemReaderTest : public ::testing::Test {
protected:
    void SetUp() { _reader = 0; }

    void TearDown()
    {
        if (_reader) {
            delete _reader;
        }
    }

    template <std::size_t n, typename R>
    void initReader(const R (&array)[n])
    {

        if (_reader) {
            delete _reader;
        }
        _reader = new MemReader(array);
    }

    void expectParams(std::size_t read, std::size_t left, void* data = nullptr)
    {
        EXPECT_EQ(read, _reader->sizeRead());
        EXPECT_EQ(left, _reader->sizeLeft());
        EXPECT_EQ(read + left, _reader->size());
        bool isEmpty = read == _reader->size();
        EXPECT_EQ(isEmpty, _reader->isEmpty());
        if (data) {
            EXPECT_EQ((uint8_t*)data, _reader->current());
        }
    }

    void skip(std::size_t size) { _reader->skip(size); }

    void peek(void* dest, std::size_t size, std::size_t offset) { _reader->peek(dest, size, offset); }

    void read(void* dest, std::size_t size) { _reader->read(dest, size); }

    void expectNextUint8(uint8_t expected)
    {
        uint8_t value;
        _reader->read(&value, 1);
        EXPECT_EQ(expected, value);
    }

    void expectNextVarUint(uint64_t expected)
    {
        uint64_t value;
        bool rv = _reader->readVarUint(&value);
        EXPECT_TRUE(rv);
        EXPECT_EQ(expected, value);
    }

    template <std::size_t n, typename R>
    void expectNextData(const R (&array)[n])
    {
        std::size_t dataSize = sizeof(R) * n;
        R temp[n];
        _reader->read(temp, dataSize);
        EXPECT_EQ_MEM(array, temp, dataSize);
    }

    template <std::size_t n>
    void makeVarUintTest(std::uint64_t value, const uint8_t (&array)[n])
    {
        initReader(array);
        expectNextVarUint(value);
        expectParams(n, 0);
    }

    MemReader* _reader;
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

TEST_F(MemReaderTest, varUint1)
{
    uint8_t data1[1] = {0};
    uint8_t data2[1] = {240};
    makeVarUintTest(0, data1);
    makeVarUintTest(240, data2);
}

TEST_F(MemReaderTest, varUint2)
{
    uint8_t data1[2] = {241, 1};
    uint8_t data2[2] = {248, 255};
    makeVarUintTest(241, data1);
    makeVarUintTest(2287, data2);
}

TEST_F(MemReaderTest, varUint3)
{
    uint8_t data1[3] = {249, 0, 0};
    uint8_t data2[3] = {249, 255, 255};
    makeVarUintTest(2288, data1);
    makeVarUintTest(67823, data2);
}

TEST_F(MemReaderTest, varUint4)
{
    uint8_t data1[4] = {250, 0x01, 0x08, 0xf0};
    uint8_t data2[4] = {250, 255, 255, 255};
    makeVarUintTest(67824, data1);
    makeVarUintTest(16777215, data2);
}

TEST_F(MemReaderTest, varUint5)
{
    uint8_t data1[5] = {251, 1, 0, 0, 0};
    uint8_t data2[5] = {251, 255, 255, 255, 255};
    makeVarUintTest(16777216, data1);
    makeVarUintTest(4294967295, data2);
}

TEST_F(MemReaderTest, varUint6)
{
    uint8_t data1[6] = {252, 1, 0, 0, 0, 0};
    uint8_t data2[6] = {252, 255, 255, 255, 255, 255};
    makeVarUintTest(4294967296, data1);
    makeVarUintTest(1099511627775, data2);
}

TEST_F(MemReaderTest, varUint7)
{
    uint8_t data1[7] = {253, 1, 0, 0, 0, 0, 0};
    uint8_t data2[7] = {253, 255, 255, 255, 255, 255, 255};
    makeVarUintTest(1099511627776, data1);
    makeVarUintTest(281474976710655, data2);
}

TEST_F(MemReaderTest, varUint8)
{
    uint8_t data1[8] = {254, 1, 0, 0, 0, 0, 0, 0};
    uint8_t data2[8] = {254, 255, 255, 255, 255, 255, 255, 255};
    makeVarUintTest(281474976710656, data1);
    makeVarUintTest(72057594037927935, data2);
}

TEST_F(MemReaderTest, varUint9)
{
    uint8_t data1[9] = {255, 1, 0, 0, 0, 0, 0, 0, 0};
    uint8_t data2[9] = {255, 255, 255, 255, 255, 255, 255, 255, 255};
    makeVarUintTest(72057594037927936, data1);
    makeVarUintTest(18446744073709551615u, data2);
}

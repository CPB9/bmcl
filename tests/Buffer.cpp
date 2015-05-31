#include "bmcl/Buffer.h"

#include "BmclTest.h"

#include <memory>

using namespace bmcl;

class BufferTest : public ::testing::Test {
protected:
    void SetUp()
    {
        _buf.reset(0);
    }

    void TearDown()
    {
        _buf.reset();
    }

    void init()
    {
        _buf.reset(new Buffer);
    }

    void init(std::size_t size)
    {
        _buf.reset(new Buffer(size));
    }

    void expectSize(std::size_t size)
    {
        EXPECT_EQ(size, _buf->size());
    }

    void expectCapacity(std::size_t capacity)
    {
        EXPECT_EQ(capacity, _buf->capacity());
    }

    template <std::size_t n, typename R>
    void expectData(const R(&array)[n])
    {
        assert(_buf);
        EXPECT_EQ_MEM(array, _buf->start(), sizeof(R) * n);
    }

    template <std::size_t n, typename R>
    void write(const R(&array)[n])
    {
        assert(_buf);
        _buf->write(array, sizeof(R) * n);
    }

    void writeUint8(uint8_t value)
    {
        assert(_buf);
        _buf->writeUint8(value);
    }

    std::unique_ptr<Buffer> _buf;
};

TEST_F(BufferTest, initEmpty)
{
    init();
    expectSize(0);
    expectCapacity(0);
}

TEST_F(BufferTest, initSize)
{
    init(125);
    expectSize(125);
    expectCapacity(125);
}

TEST_F(BufferTest, write)
{
    init();
    const uint8_t testData[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
    write(testData);
    expectSize(10);
    expectData(testData);
}

TEST_F(BufferTest, writeSeveral)
{
    init();
    const uint8_t expected[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
    writeUint8(1);
    writeUint8(2);
    writeUint8(3);
    writeUint8(4);
    writeUint8(5);
    writeUint8(6);
    writeUint8(7);
    writeUint8(8);
    writeUint8(9);
    writeUint8(0);
    expectSize(10);
    expectData(expected);
}

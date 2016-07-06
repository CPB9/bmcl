#include "bmcl/Endian.h"
#include "bmcl/MemWriter.h"

#include "BmclTest.h"

using namespace bmcl;

class MemWriterTest : public ::testing::Test {
protected:
    MemWriterTest()
        : _writer(0)
        , _data(0)
    {
    }

    void SetUp()
    {
        _writer = 0;
        _data = 0;
    }

    void TearDown()
    {
        if (_writer) {
            delete _writer;
        }
        if (_data) {
            delete [] _data;
        }
    }

    template <std::size_t n, typename R>
    void initWriter(R(&array)[n])
    {
        assert(_writer == 0);
        _writer = new MemWriter(array);
    }

    template <std::size_t n, typename R>
    void initWriterWithSizeOf(const R(&array)[n])
    {
        (void)array;
        assert(_writer == 0);
        _data = new uint8_t[sizeof(R) * n];
        _writer = new MemWriter(_data, sizeof(R) * n);
    }

    void initWriterWithSize(std::size_t size)
    {
        assert(_writer == 0);
        _data = new uint8_t[size];
        _writer = new MemWriter(_data, size);
    }

    template <std::size_t n, typename R>
    void expect(const R(&array)[n])
    {
        EXPECT_EQ_MEM(array, _writer->start(), sizeof(R) * n);
    }

    void expectSizes(std::size_t used, std::size_t left)
    {
        EXPECT_EQ(used == 0, _writer->isEmpty());
        EXPECT_EQ(left == 0, _writer->isFull());
        EXPECT_EQ(used, _writer->sizeUsed());
        EXPECT_EQ(left, _writer->writableSize());
        EXPECT_EQ(used + left, _writer->maxSize());
        bool isFull = used == _writer->maxSize();
        EXPECT_EQ(isFull, _writer->isFull());
        EXPECT_EQ(_writer->start() + used, _writer->current());
    }

    template <std::size_t n, typename R>
    void append(const R(&array)[n])
    {
        _writer->write(array);
    }

    void appendUint8(uint8_t data)
    {
        _writer->write(&data, 1);
    }

    void fillUp(uint8_t data)
    {
        _writer->fillUp(data);
    }

    void fill(uint8_t data, std::size_t size)
    {
        _writer->fill(data, size);
    }

    template <std::size_t n>
    void makeVarUintTest(std::uint64_t value, const uint8_t (&array)[n])
    {
        uint8_t* data = new uint8_t[n];
        MemWriter writer(data, n);
        bool rv = writer.writeVarUint(value);
        EXPECT_TRUE(rv);
        EXPECT_EQ(n, writer.sizeUsed());
        EXPECT_EQ(0, writer.sizeLeft());
        EXPECT_EQ_MEM(array, writer.start(), n);
        delete [] data;
    }


private:
    MemWriter* _writer;
    uint8_t* _data;
};

TEST_F(MemWriterTest, init)
{
    initWriterWithSize(100);
    expectSizes(0, 100);
}

TEST_F(MemWriterTest, with_dest)
{
    uint8_t data[5] = {0xaa, 0x11, 0x22, 0xbb, 0xff};
    uint8_t dest[5] = {0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t expected[5] = {0xaa, 0x11, 0x22, 0xbb, 0xff};
    initWriter(dest);
    expectSizes(0, 5);
    append(data);
    expectSizes(5, 0);
    EXPECT_EQ_ARRAYS(expected, dest);
}

TEST_F(MemWriterTest, append)
{
    uint8_t data[4] = {0xaa, 0xbb, 0xcc, 0xdd};
    uint8_t expected[4] = {0xaa, 0xbb, 0xcc, 0xdd};
    initWriterWithSize(7);
    append(data);
    expect(expected);
    expectSizes(4, 3);
}

TEST_F(MemWriterTest, append_full)
{
    uint8_t data1[4] = {0xaa, 0xbb, 0xcc, 0xdd};
    uint8_t data2[4] = {0x11, 0x22, 0x33, 0x44};
    uint8_t expected[8] = {0xaa, 0xbb, 0xcc, 0xdd, 0x11, 0x22, 0x33, 0x44};
    initWriterWithSizeOf(expected);
    append(data1);
    append(data2);
    expect(expected);
    expectSizes(8, 0);
}

TEST_F(MemWriterTest, fill_up)
{
    uint8_t expected[8] = {0xaa, 0xff, 0xcc, 0xdd, 0x11, 0x11, 0x11, 0x11};
    initWriterWithSizeOf(expected);
    appendUint8(0xaa);
    appendUint8(0xff);
    appendUint8(0xcc);
    appendUint8(0xdd);
    fillUp(0x11);
    expect(expected);
    expectSizes(8, 0);
}

TEST_F(MemWriterTest, fill)
{
    uint8_t expected[6] = {0xaa, 0xaa, 0xcc, 0xbb, 0xbb, 0xbb};
    initWriterWithSizeOf(expected);
    fill(0xaa, 2);
    fill(0xcc, 1);
    fill(0xbb, 3);
    expect(expected);
    expectSizes(6, 0);
}

TEST_F(MemWriterTest, append_uint8)
{
    uint8_t expected[5] = {0x11, 0x22, 0x33, 0x44, 0x55};
    initWriterWithSizeOf(expected);
    appendUint8(0x11);
    appendUint8(0x22);
    appendUint8(0x33);
    appendUint8(0x44);
    appendUint8(0x55);
    expect(expected);
    expectSizes(5, 0);
}

//TODO: refact

TEST_F(MemWriterTest, varUint1)
{
    makeVarUintTest(0, {0});
    makeVarUintTest(240, {240});
}

TEST_F(MemWriterTest, varUint2)
{
    makeVarUintTest(241, {241, 1});
    makeVarUintTest(2287, {248, 255});
}

TEST_F(MemWriterTest, varUint3)
{
    makeVarUintTest(2288, {249, 0, 0});
    makeVarUintTest(67823, {249, 255, 255});
}

TEST_F(MemWriterTest, varUint4)
{
    makeVarUintTest(67824, {250, 0x01, 0x08, 0xf0});
    makeVarUintTest(16777215, {250, 255, 255, 255});
}

TEST_F(MemWriterTest, varUint5)
{
    makeVarUintTest(16777216, {251, 1, 0, 0, 0});
    makeVarUintTest(4294967295, {251, 255, 255, 255, 255});
}

TEST_F(MemWriterTest, varUint6)
{
    makeVarUintTest(4294967296, {252, 1, 0, 0, 0, 0});
    makeVarUintTest(1099511627775, {252, 255, 255, 255, 255, 255});
}

TEST_F(MemWriterTest, varUint7)
{
    makeVarUintTest(1099511627776, {253, 1, 0, 0, 0, 0, 0});
    makeVarUintTest(281474976710655, {253, 255, 255, 255, 255, 255, 255});
}

TEST_F(MemWriterTest, varUint8)
{
    makeVarUintTest(281474976710656, {254, 1, 0, 0, 0, 0, 0, 0});
    makeVarUintTest(72057594037927935, {254, 255, 255, 255, 255, 255, 255, 255});
}

TEST_F(MemWriterTest, varUint9)
{
    makeVarUintTest(72057594037927936, {255, 1, 0, 0, 0, 0, 0, 0, 0});
    makeVarUintTest(18446744073709551615u, {255, 255, 255, 255, 255, 255, 255, 255, 255});
}

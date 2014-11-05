#include "bmcl/core/endian.h"
#include "bmcl/core/memwriter.h"

#include "bmcl-test.h"

class MemWriterTest: public ::testing::Test
{
protected:
    void SetUp()
    {
        _writer = 0;
    }

    void TearDown()
    {
        if (_writer) {
            bmcl_memwriter_destroy(_writer);
        }
    }

    template <std::size_t n, typename R>
    void initWriter(R (&array)[n])
    {
        assert(_writer == 0);
        _writer = bmcl_memwriter_create_with_dest(array, sizeof(R) * n);
    }

    template <std::size_t n, typename R>
    void initWriterWithSizeOf(const R (&array)[n])
    {
        (void)array;
        assert(_writer == 0);
        _writer = bmcl_memwriter_create(sizeof(R) * n);
    }

    void initWriterWithSize(std::size_t size)
    {
        assert(_writer == 0);
        _writer = bmcl_memwriter_create(size);
    }

    template <std::size_t n, typename R>
    void expect(const R (&array)[n])
    {
        EXPECT_EQ_MEM(array, bmcl_memwriter_ptr(_writer), sizeof(R) * n);
    }

    void expectSizes(std::size_t used, std::size_t left)
    {
        EXPECT_EQ(used, bmcl_memwriter_size(_writer));
        EXPECT_EQ(left, bmcl_memwriter_size_left(_writer));
        EXPECT_EQ(used + left, bmcl_memwriter_max_size(_writer));
        bool isFull = used == bmcl_memwriter_max_size(_writer);
        EXPECT_EQ(isFull, bmcl_memwriter_is_full(_writer));
        EXPECT_EQ((uint8_t*)bmcl_memwriter_ptr(_writer) + used, bmcl_memwriter_current_ptr(_writer));
    }

    template <std::size_t n, typename R>
    void append(const R (&array)[n])
    {
        bmcl_memwriter_write(_writer, array, sizeof(R) * n);
    }

    void appendUint8(uint8_t data)
    {
        bmcl_memwriter_write_uint8(_writer, data);
    }

    void fillUp(uint8_t data)
    {
        bmcl_memwriter_fillup(_writer, data);
    }

    void fill(uint8_t data, std::size_t size)
    {
        bmcl_memwriter_fill(_writer, data, size);
    }

private:
    bmcl_memwriter_t* _writer;
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
    append(data);
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

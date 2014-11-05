#include "bmcl/core/ringbuf.h"

#include "bmcl-test.h"

class RingBufTest : public ::testing::Test {
protected:
    void SetUp()
    {
        _ringbuf = 0;
    }

    void TearDown()
    {
        if (_ringbuf) {
            bmcl_ringbuf_destroy(_ringbuf);
        }
    }

    template <std::size_t n, typename R>
    void initRingBufWithSizeOf(const R (&array)[n])
    {
        (void)array;
        assert(_ringbuf == 0);
        _ringbuf = bmcl_ringbuf_create(sizeof(R) * n);
    }

    void initRingBufWithSize(std::size_t size)
    {
        assert(_ringbuf == 0);
        _ringbuf = bmcl_ringbuf_create(size);
    }

    template <std::size_t n, typename R>
    void append(const R (&array)[n])
    {
        bmcl_ringbuf_write(_ringbuf, array, sizeof(R) * n);
    }

    void erase(std::size_t size)
    {
        bmcl_ringbuf_erase(_ringbuf, size);
    }

    void clear()
    {
        bmcl_ringbuf_clear(_ringbuf);
    }

    void appendByte(uint8_t byte)
    {
        bmcl_ringbuf_write_uint8(_ringbuf, byte);
    }

    uint8_t readByte()
    {
        return bmcl_ringbuf_read_uint8(_ringbuf);
    }

    void peek(void* dest, std::size_t size, std::size_t offset = 0)
    {
        bmcl_ringbuf_peek(_ringbuf, dest, size, offset);
    }

    void read(void* dest, std::size_t size)
    {
        bmcl_ringbuf_read(_ringbuf, dest, size);
    }

    void expectFreeSpace(std::size_t freeSpace)
    {
        EXPECT_EQ(bmcl_ringbuf_get_free_space(_ringbuf), freeSpace);
    }

    void expectFull()
    {
        EXPECT_TRUE(bmcl_ringbuf_is_full(_ringbuf));
    }

    void expectEmpty()
    {
        EXPECT_TRUE(bmcl_ringbuf_is_empty(_ringbuf));
    }

    template <std::size_t n, typename R>
    void expect(const R (&array)[n])
    {
        std::size_t dataSize = sizeof(R) * n;
        uint8_t temp[dataSize];
        bmcl_ringbuf_peek(_ringbuf, temp, dataSize, 0);
        EXPECT_EQ(dataSize, bmcl_ringbuf_get_used_space(_ringbuf));
        EXPECT_EQ_MEM(array, temp, dataSize);
    }

private:
    bmcl_ringbuf_t* _ringbuf;
};

TEST_F(RingBufTest, init)
{
    initRingBufWithSize(4);
    expectFreeSpace(4);
}

TEST_F(RingBufTest, append_one_byte)
{
    uint8_t data[1] = {0xaf};
    uint8_t expected[1] = {0xaf};
    initRingBufWithSize(3);
    append(data);
    expectFreeSpace(2);
    expect(expected);
}

TEST_F(RingBufTest, append_several)
{
    uint8_t data[3] = {0x46, 0xa5, 0xfd};
    uint8_t expected[3] = {0x46, 0xa5, 0xfd};
    initRingBufWithSize(5);
    append(data);
    expectFreeSpace(2);
    expect(expected);
}

TEST_F(RingBufTest, append_until_full)
{
    uint8_t data[4] = {0x11, 0xbb, 0xaa, 0x47};
    uint8_t expected[4] = {0x11, 0xbb, 0xaa, 0x47};
    initRingBufWithSizeOf(expected);
    append(data);
    expectFull();
    expect(expected);
}

TEST_F(RingBufTest, overwrite_one_byte)
{
    uint8_t data[4] = {0x46, 0xa5, 0xfd, 0xcc};
    uint8_t expected[3] = {0xa5, 0xfd, 0xcc};
    initRingBufWithSizeOf(expected);
    appendByte(data[0]);
    appendByte(data[1]);
    appendByte(data[2]);
    appendByte(data[3]);
    expectFull();
    expect(expected);
}

TEST_F(RingBufTest, overwrite_twice)
{
    uint8_t data1[2] = {0x46, 0xa5};
    uint8_t data2[2] = {0xfd, 0xbb};
    uint8_t data3[1] = {0xcc};
    uint8_t expected[2] = {0xbb, 0xcc};
    initRingBufWithSizeOf(expected);
    append(data1);
    append(data2);
    append(data3);
    expectFull();
    expect(expected);
}

TEST_F(RingBufTest, overwrite_split)
{
    uint8_t data1[2] = {0x46, 0xa5};
    uint8_t data2[2] = {0x58, 0xfa};
    uint8_t expected[3] = {0xa5, 0x58, 0xfa};
    initRingBufWithSizeOf(expected);
    append(data1);
    append(data2);
    expectFull();
    expect(expected);
}

TEST_F(RingBufTest, read_one)
{
    uint8_t data[3] = {0xaa, 0xbb, 0xcc};
    uint8_t expected[2] = {0xbb, 0xcc};
    initRingBufWithSizeOf(data);
    append(data);
    readByte();
    expectFreeSpace(1);
    expect(expected);
}

TEST_F(RingBufTest, read_until_free)
{
    uint8_t data[2] = {0xbb, 0xcc};
    initRingBufWithSizeOf(data);
    append(data);
    readByte();
    readByte();
    expectEmpty();
}

TEST_F(RingBufTest, erase)
{
    uint8_t data[4] = {0xaa, 0xbb, 0xcc, 0xdd};
    uint8_t expected[2] = {0xcc, 0xdd};
    initRingBufWithSizeOf(data);
    append(data);
    erase(2);
    expect(expected);
}

TEST_F(RingBufTest, erase_all)
{
    uint8_t data[4] = {0xaa, 0xbb, 0xcc, 0xdd};
    initRingBufWithSizeOf(data);
    append(data);
    erase(4);
    expectEmpty();
}

TEST_F(RingBufTest, peek_with_offset)
{
    uint8_t data1[4] = {0xaa, 0xbb, 0xcc, 0xdd};
    uint8_t data2[2] = {0xaa, 0xbb};
    uint8_t dest[2] = {0x00, 0x00};
    uint8_t expected[2] = {0xaa, 0xbb};
    initRingBufWithSize(4);
    append(data1);
    append(data2);
    peek(dest, 2, 2);
    EXPECT_EQ_ARRAYS(expected, dest);
}

TEST_F(RingBufTest, read)
{
    uint8_t data[4] = {0xaa, 0xbb, 0xcc, 0xdd};
    uint8_t dest[4] = {0x00, 0x00, 0x00, 0x00};
    uint8_t expected[4] = {0xaa, 0xbb, 0xcc, 0xdd};
    initRingBufWithSizeOf(expected);
    append(data);
    read(dest, 4);
    expectEmpty();
    EXPECT_EQ_ARRAYS(expected, dest);
}

TEST_F(RingBufTest, clear)
{
    uint8_t data[4] = {0xaa, 0xbb, 0xcc, 0xdd};
    initRingBufWithSizeOf(data);
    append(data);
    clear();
    expectEmpty();
}

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
            ringbuf_destroy(_ringbuf);
        }
    }

    void initRingBuf(size_t size)
    {
        assert(_ringbuf == 0);
        _ringbuf = ringbuf_create(size);
    }

    void append(void* data, size_t size)
    {
        ringbuf_write(_ringbuf, data, size);
    }

    void erase(size_t size)
    {
        ringbuf_erase(_ringbuf, size);
    }

    void clear()
    {
        ringbuf_clear(_ringbuf);
    }

    void appendByte(uint8_t byte)
    {
        ringbuf_write_uint8(_ringbuf, byte);
    }

    uint8_t readByte()
    {
        return ringbuf_read_uint8(_ringbuf);
    }

    void peek(void* dest, size_t size, size_t offset = 0)
    {
        ringbuf_peek(_ringbuf, dest, size, offset);
    }

    void read(void* dest, size_t size)
    {
        ringbuf_read(_ringbuf, dest, size);
    }

    void expectFreeSpace(size_t freeSpace)
    {
        EXPECT_EQ(ringbuf_get_free_space(_ringbuf), freeSpace);
    }

    void expectFull()
    {
        EXPECT_TRUE(ringbuf_is_full(_ringbuf));
    }

    void expectEmpty()
    {
        EXPECT_TRUE(ringbuf_is_empty(_ringbuf));
    }

    void expectData(void* expected, size_t size)
    {
        uint8_t* temp = new uint8_t[size];
        ringbuf_peek(_ringbuf, temp, size, 0);
        EXPECT_EQ(size, ringbuf_get_used_space(_ringbuf));
        EXPECT_EQ_MEM(expected, temp, size);
        delete [] temp;
    }

private:
    ringbuf_t* _ringbuf;
};

TEST_F(RingBufTest, init)
{
    initRingBuf(4);
    expectFreeSpace(4);
}

TEST_F(RingBufTest, appendOneByte)
{
    uint8_t data[1] = {0xaf};
    uint8_t expected[1] = {0xaf};
    initRingBuf(3);
    append(data, 1);
    expectFreeSpace(2);
    expectData(expected, 1);
}

TEST_F(RingBufTest, appendSeveral)
{
    uint8_t data[3] = {0x46, 0xa5, 0xfd};
    uint8_t expected[3] = {0x46, 0xa5, 0xfd};
    initRingBuf(5);
    append(data, 3);
    expectFreeSpace(2);
    expectData(expected, 3);
}

TEST_F(RingBufTest, appendUntilFull)
{
    uint8_t data[4] = {0x11, 0xbb, 0xaa, 0x47};
    uint8_t expected[4] = {0x11, 0xbb, 0xaa, 0x47};
    initRingBuf(4);
    append(data, 4);
    expectFull();
    expectData(expected, 4);
}

TEST_F(RingBufTest, overwriteOneByte)
{
    uint8_t data[4] = {0x46, 0xa5, 0xfd, 0xcc};
    uint8_t expected[3] = {0xa5, 0xfd, 0xcc};
    initRingBuf(3);
    appendByte(data[0]);
    appendByte(data[1]);
    appendByte(data[2]);
    appendByte(data[3]);
    expectFull();
    expectData(expected, 3);
}

TEST_F(RingBufTest, overwriteTwice)
{
    uint8_t data[5] = {0x46, 0xa5, 0xfd, 0xbb, 0xcc};
    uint8_t expected[2] = {0xbb, 0xcc};
    initRingBuf(2);
    append(data, 2);
    append(data + 2, 2);
    append(data + 4, 1);
    expectFull();
    expectData(expected, 2);
}

TEST_F(RingBufTest, overwriteSplit)
{
    uint8_t data1[2] = {0x46, 0xa5};
    uint8_t data2[2] = {0x58, 0xfa};
    uint8_t expected[3] = {0xa5, 0x58, 0xfa};
    initRingBuf(3);
    append(data1, 2);
    append(data2, 2);
    expectFull();
    expectData(expected, 3);
}

TEST_F(RingBufTest, readOne)
{
    uint8_t data[3] = {0xaa, 0xbb, 0xcc};
    uint8_t expected[2] = {0xbb, 0xcc};
    initRingBuf(3);
    append(data, 3);
    readByte();
    expectFreeSpace(1);
    expectData(expected, 2);
}

TEST_F(RingBufTest, readUntilFree)
{
    uint8_t data[2] = {0xbb, 0xcc};
    initRingBuf(2);
    append(data, 2);
    readByte();
    readByte();
    expectEmpty();
}

TEST_F(RingBufTest, erase)
{
    uint8_t data[4] = {0xaa, 0xbb, 0xcc, 0xdd};
    uint8_t expected[2] = {0xcc, 0xdd};
    initRingBuf(4);
    append(data, 4);
    erase(2);
    expectData(expected, 2);
}

TEST_F(RingBufTest, eraseAll)
{
    uint8_t data[4] = {0xaa, 0xbb, 0xcc, 0xdd};
    initRingBuf(4);
    append(data, 4);
    erase(4);
    expectEmpty();
}

TEST_F(RingBufTest, peekWithOffset)
{
    uint8_t data[4] = {0xaa, 0xbb, 0xcc, 0xdd};
    uint8_t dest[2] = {0x00, 0x00};
    uint8_t expected[2] = {0xaa, 0xbb};
    initRingBuf(4);
    append(data, 4);
    append(data, 2);
    peek(dest, 2, 2);
    EXPECT_EQ_MEM(expected, dest, 2);
}

TEST_F(RingBufTest, read)
{
    uint8_t data[4] = {0xaa, 0xbb, 0xcc, 0xdd};
    uint8_t dest[4] = {0x00, 0x00, 0x00, 0x00};
    uint8_t expected[4] = {0xaa, 0xbb, 0xcc, 0xdd};
    initRingBuf(4);
    append(data, 4);
    read(dest, 4);
    expectEmpty();
    EXPECT_EQ_MEM(expected, dest, 2);
}

TEST_F(RingBufTest, clear)
{
    uint8_t data[4] = {0xaa, 0xbb, 0xcc, 0xdd};
    initRingBuf(4);
    append(data, 4);
    clear();
    expectEmpty();
}

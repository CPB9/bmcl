#include "ringbuf.h"

#include "gtest/gtest.h"

static void print_mem(const void* mem, std::size_t size)
{
    for (std::size_t i = 0; i < size; i++) {
        std::cout << std::hex << (std::size_t) * ((uint8_t*)mem + i);
    }
}

static void check_eq_mem(const uint8_t* expected, const uint8_t* actual, size_t size, const char* fname,
                         std::size_t line)
{
    if (memcmp(expected, actual, size) != 0) {
        ADD_FAILURE_AT(fname, line);
        std::cout << "Expected memory: ";
        print_mem(expected, size);
        std::cout << std::endl << "Actual memory:   ";
        print_mem(actual, size);
        std::cout << std::endl;
    }
}

#define EXPECT_EQ_MEM(expected, actual, size)                                                                          \
    check_eq_mem((const uint8_t*)expected, (const uint8_t*)actual, size, __FILE__, __LINE__)

class RingBufTest : public ::testing::Test {
protected:
    void SetUp()
    {
        ringbuf_clear(&_ringbuf);
    }

    void TearDown()
    {
        delete[] buffer;
    }

    void initRingBuf(uint size)
    {
        buffer = new uint8_t[size + 10];
        memset(buffer, 0, size + 10);
        ringbuf_init(&_ringbuf, buffer + 5, size);
    }

    void append(void* data, uint size)
    {
        ringbuf_append(&_ringbuf, data, size);
    }

    void erase(uint size)
    {
        ringbuf_erase(&_ringbuf, size);
    }

    void appendByte(uint8_t byte)
    {
        ringbuf_append_uint8(&_ringbuf, byte);
    }

    uint8_t readByte()
    {
        return ringbuf_read_uint8(&_ringbuf);
    }

    void expectFreeSpace(uint freeSpace)
    {
        EXPECT_EQ(ringbuf_get_free_space(&_ringbuf), freeSpace);
    }

    void expectFull()
    {
        EXPECT_TRUE(ringbuf_is_full(&_ringbuf));
    }

    void expectEmpty()
    {
        EXPECT_TRUE(ringbuf_is_empty(&_ringbuf));
    }

    void expectData(void* expected, uint size)
    {
        uint8_t temp[size];
        ringbuf_peek(&_ringbuf, temp, size, 0);
        EXPECT_EQ(size, ringbuf_get_used_space(&_ringbuf));
        EXPECT_EQ_MEM(expected, temp, size);
    }

private:
    ringbuf_t _ringbuf;
    uint8_t* buffer;
};

TEST_F(RingBufTest, init)
{
    initRingBuf(4);
    expectFreeSpace(4);
}

TEST_F(RingBufTest, appendOneByte)
{
    uint data[1] = {0xaf};
    uint8_t expected[3] = {0xaf};
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
    append(data, 2);
    append(data + 2, 2);
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

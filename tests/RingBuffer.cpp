#include "bmcl/RingBuffer.h"

#include "BmclTest.h"

using namespace bmcl;

class RingBufferTest : public ::testing::Test {
protected:
    RingBufferTest()
        : _ringbuf(0)
    {
    }

    void SetUp() { _ringbuf = 0; }

    void TearDown()
    {
        if (_ringbuf) {
            delete _ringbuf;
        }
    }

    template <std::size_t n, typename R>
    void initRingBufferWithSizeOf(const R (&array)[n])
    {
        (void)array;
        assert(_ringbuf == 0);
        _ringbuf = new RingBuffer(sizeof(R) * n);
    }

    void initRingBufferWithSize(std::size_t size)
    {
        assert(_ringbuf == 0);
        _ringbuf = new RingBuffer(size);
    }

    template <std::size_t n, typename R>
    void append(const R (&array)[n])
    {
        _ringbuf->write(array, sizeof(R) * n);
    }

    void erase(std::size_t size) { _ringbuf->erase(size); }

    void clear() { _ringbuf->clear(); }

    void appendByte(uint8_t byte) { _ringbuf->write(&byte, 1); }

    uint8_t readByte()
    {
        uint8_t data;
        _ringbuf->read(&data, 1);
        return data;
    }

    void peek(void* dest, std::size_t size, std::size_t offset = 0) { _ringbuf->peek(dest, size, offset); }

    void read(void* dest, std::size_t size) { _ringbuf->read(dest, size); }

    void expectFreeSpace(std::size_t freeSpace) { EXPECT_EQ(_ringbuf->freeSpace(), freeSpace); }

    void expectFull() { EXPECT_TRUE(_ringbuf->isFull()); }

    void expectEmpty() { EXPECT_TRUE(_ringbuf->isEmpty()); }

    template <std::size_t n, typename R>
    void expect(const R (&array)[n])
    {
        std::size_t dataSize = sizeof(R) * n;
        R temp[n];
        _ringbuf->peek(temp, dataSize);
        EXPECT_EQ(dataSize, _ringbuf->usedSpace());
        EXPECT_EQ_MEM(array, temp, dataSize);
    }

private:
    RingBuffer* _ringbuf;
};

TEST_F(RingBufferTest, init)
{
    initRingBufferWithSize(4);
    expectFreeSpace(4);
}

TEST_F(RingBufferTest, append_one_byte)
{
    uint8_t data[1] = {0xaf};
    uint8_t expected[1] = {0xaf};
    initRingBufferWithSize(3);
    append(data);
    expectFreeSpace(2);
    expect(expected);
}

TEST_F(RingBufferTest, append_several)
{
    uint8_t data[3] = {0x46, 0xa5, 0xfd};
    uint8_t expected[3] = {0x46, 0xa5, 0xfd};
    initRingBufferWithSize(5);
    append(data);
    expectFreeSpace(2);
    expect(expected);
}

TEST_F(RingBufferTest, append_until_full)
{
    uint8_t data[4] = {0x11, 0xbb, 0xaa, 0x47};
    uint8_t expected[4] = {0x11, 0xbb, 0xaa, 0x47};
    initRingBufferWithSizeOf(expected);
    append(data);
    expectFull();
    expect(expected);
}

TEST_F(RingBufferTest, overwrite_one_byte)
{
    uint8_t data[4] = {0x46, 0xa5, 0xfd, 0xcc};
    uint8_t expected[3] = {0xa5, 0xfd, 0xcc};
    initRingBufferWithSizeOf(expected);
    appendByte(data[0]);
    appendByte(data[1]);
    appendByte(data[2]);
    appendByte(data[3]);
    expectFull();
    expect(expected);
}

TEST_F(RingBufferTest, overwrite_twice)
{
    uint8_t data1[2] = {0x46, 0xa5};
    uint8_t data2[2] = {0xfd, 0xbb};
    uint8_t data3[1] = {0xcc};
    uint8_t expected[2] = {0xbb, 0xcc};
    initRingBufferWithSizeOf(expected);
    append(data1);
    append(data2);
    append(data3);
    expectFull();
    expect(expected);
}

TEST_F(RingBufferTest, overwrite_split)
{
    uint8_t data1[2] = {0x46, 0xa5};
    uint8_t data2[2] = {0x58, 0xfa};
    uint8_t expected[3] = {0xa5, 0x58, 0xfa};
    initRingBufferWithSizeOf(expected);
    append(data1);
    append(data2);
    expectFull();
    expect(expected);
}

TEST_F(RingBufferTest, read_one)
{
    uint8_t data[3] = {0xaa, 0xbb, 0xcc};
    uint8_t expected[2] = {0xbb, 0xcc};
    initRingBufferWithSizeOf(data);
    append(data);
    readByte();
    expectFreeSpace(1);
    expect(expected);
}

TEST_F(RingBufferTest, read_until_free)
{
    uint8_t data[2] = {0xbb, 0xcc};
    initRingBufferWithSizeOf(data);
    append(data);
    readByte();
    readByte();
    expectEmpty();
}

TEST_F(RingBufferTest, erase)
{
    uint8_t data[4] = {0xaa, 0xbb, 0xcc, 0xdd};
    uint8_t expected[2] = {0xcc, 0xdd};
    initRingBufferWithSizeOf(data);
    append(data);
    erase(2);
    expect(expected);
}

TEST_F(RingBufferTest, erase_all)
{
    uint8_t data[4] = {0xaa, 0xbb, 0xcc, 0xdd};
    initRingBufferWithSizeOf(data);
    append(data);
    erase(4);
    expectEmpty();
}

TEST_F(RingBufferTest, peek_with_offset)
{
    uint8_t data1[4] = {0xaa, 0xbb, 0xcc, 0xdd};
    uint8_t data2[2] = {0xaa, 0xbb};
    uint8_t dest[2] = {0x00, 0x00};
    uint8_t expected[2] = {0xaa, 0xbb};
    initRingBufferWithSize(4);
    append(data1);
    append(data2);
    peek(dest, 2, 2);
    EXPECT_EQ_ARRAYS(expected, dest);
}

TEST_F(RingBufferTest, read)
{
    uint8_t data[4] = {0xaa, 0xbb, 0xcc, 0xdd};
    uint8_t dest[4] = {0x00, 0x00, 0x00, 0x00};
    uint8_t expected[4] = {0xaa, 0xbb, 0xcc, 0xdd};
    initRingBufferWithSizeOf(expected);
    append(data);
    read(dest, 4);
    expectEmpty();
    EXPECT_EQ_ARRAYS(expected, dest);
}

TEST_F(RingBufferTest, clear)
{
    uint8_t data[4] = {0xaa, 0xbb, 0xcc, 0xdd};
    initRingBufferWithSizeOf(data);
    append(data);
    clear();
    expectEmpty();
}

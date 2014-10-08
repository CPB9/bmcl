#include "bmcl/endian.h"
#include "bmcl/writer.h"
#include "bmcl/memwriter.h"
#include "bmcl/ringbuf.h"

#include "bmcl-test.h"

class Writer {
public:
    virtual ~Writer()
    {
    }
    writer_t* get()
    {
        return _writer;
    }

    virtual size_t dataSize() const = 0;

    virtual void copyData(void* dest) const = 0;

protected:
    writer_t* _writer;
};

class MemWriter : public Writer {
public:
    MemWriter(size_t size)
    {
        _writer = new writer_t;
        _memwriter = new memwriter_t;
        _data = new uint8_t[size];
        memset(_data, 0, size);
        memwriter_init(_memwriter, _data, size);
        memwriter_init_writer(_memwriter, _writer);
    }

    ~MemWriter()
    {
        delete _data;
        delete _memwriter;
        delete _writer;
    }

    size_t dataSize() const
    {
        return memwriter_size(_memwriter);
    }

    void copyData(void* dest) const
    {
        memcpy(dest, memwriter_ptr(_memwriter), memwriter_size(_memwriter));
    }

private:
    uint8_t* _data;
    memwriter_t* _memwriter;
};

class RingBufWriter : public Writer {
public:
    RingBufWriter(size_t size)
    {
        _writer = new writer_t;
        _ringbuf = new ringbuf_t;
        _data = new uint8_t[size];
        memset(_data, 0, size);
        ringbuf_init(_ringbuf, _data, size);
        ringbuf_init_writer(_ringbuf, _writer);
    }

    ~RingBufWriter()
    {
        delete _data;
        delete _ringbuf;
        delete _writer;
    }

    size_t dataSize() const
    {
        return ringbuf_get_used_space(_ringbuf);
    }

    void copyData(void* dest) const
    {
        ringbuf_peek(_ringbuf, dest, dataSize(), 0);
    }

private:
    uint8_t* _data;
    ringbuf_t* _ringbuf;
};

template <typename T>
class WriterTest : public ::testing::Test {
public:
    writer_t* newWriter(size_t size)
    {
        assert(_shell == 0);
        _shell = new T(size);
        return _shell->get();
    }

protected:
    WriterTest()
        : _shell(0)
    {
    }

    void SetUp()
    {
    }

    void expectData(void* data, size_t size)
    {
        ASSERT_EQ(size, _shell->dataSize());
        uint8_t tmp[_shell->dataSize()];
        _shell->copyData(tmp);
        EXPECT_EQ_MEM(data, tmp, size);
    }

    void TearDown()
    {
        if (_shell != 0) {
            delete _shell;
        }
    }

private:
    T* _shell;
};

typedef ::testing::Types<MemWriter, RingBufWriter> WriterTypes;
TYPED_TEST_CASE(WriterTest, WriterTypes);

TYPED_TEST(WriterTest, write)
{
    uint8_t expected[7] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77};
    uint8_t chunk1[4] = {0x11, 0x22, 0x33, 0x44};
    uint8_t chunk2[3] = {0x55, 0x66, 0x77};
    auto writer = this->newWriter(7);
    writer_write(writer, chunk1, 4);
    writer_write(writer, chunk2, 3);
    this->expectData(expected, 7);
}

TYPED_TEST(WriterTest, write_uint8)
{
    uint8_t expected[5] = {0xab, 0x12, 0x53, 0x98, 0xfa};
    auto writer = this->newWriter(5);
    writer_write_uint8(writer, 0xab);
    writer_write_uint8(writer, 0x12);
    writer_write_uint8(writer, 0x53);
    writer_write_uint8(writer, 0x98);
    writer_write_uint8(writer, 0xfa);
    this->expectData(expected, 5);
}

TYPED_TEST(WriterTest, write_uint16le)
{
#ifdef BMCL_LITTLE_ENDIAN
    uint16_t expected[4] = {0x1234, 0x5678, 0x90ab, 0xcdef};
#else
    uint16_t expected[4] = {0x3412, 0x7856, 0xab90, 0xefcd};
#endif
    auto writer = this->newWriter(8);
    writer_write_uint16le(writer, 0x1234);
    writer_write_uint16le(writer, 0x5678);
    writer_write_uint16le(writer, 0x90ab);
    writer_write_uint16le(writer, 0xcdef);
    this->expectData(expected, 8);
}

TYPED_TEST(WriterTest, write_uint32le)
{
#ifdef BMCL_LITTLE_ENDIAN
    uint32_t expected[4] = {0x12345678, 0x90abcdef, 0x10293847, 0x56473829};
#else
    uint32_t expected[4] = {0x78563412, 0xefcdab90, 0x47382910, 0x29384756};
#endif
    auto writer = this->newWriter(16);
    writer_write_uint32le(writer, 0x12345678);
    writer_write_uint32le(writer, 0x90abcdef);
    writer_write_uint32le(writer, 0x10293847);
    writer_write_uint32le(writer, 0x56473829);
    this->expectData(expected, 16);
}

TYPED_TEST(WriterTest, write_uint64le)
{
#ifdef BMCL_LITTLE_ENDIAN
    uint64_t expected[2] = {0x1234567890abcdef, 0x1029384756473829};
#else
    uint64_t expected[2] = {0xefcdab9078563412, 0x2938475647382910};
#endif
    auto writer = this->newWriter(16);
    writer_write_uint64le(writer, 0x1234567890abcdef);
    writer_write_uint64le(writer, 0x1029384756473829);
    this->expectData(expected, 16);
}

TYPED_TEST(WriterTest, write_uint16be)
{
#ifdef BMCL_LITTLE_ENDIAN
    uint16_t expected[3] = {0x9584, 0x1876, 0x0573};
#else
    uint16_t expected[3] = {0x8495, 0x7618, 0x7305};
#endif
    auto writer = this->newWriter(6);
    writer_write_uint16be(writer, 0x8495);
    writer_write_uint16be(writer, 0x7618);
    writer_write_uint16be(writer, 0x7305);
    this->expectData(expected, 6);
}

TYPED_TEST(WriterTest, write_uint32be)
{
#ifdef BMCL_LITTLE_ENDIAN
    uint32_t expected[4] = {0x05846377, 0x07594412, 0x09876543};
#else
    uint32_t expected[4] = {0x77638405, 0x12445907, 0x43658709};
#endif
    auto writer = this->newWriter(12);
    writer_write_uint32be(writer, 0x77638405);
    writer_write_uint32be(writer, 0x12445907);
    writer_write_uint32be(writer, 0x43658709);
    this->expectData(expected, 12);
}

TYPED_TEST(WriterTest, write_uint64be)
{
#ifdef BMCL_LITTLE_ENDIAN
    uint64_t expected[2] = {0x97a950b1047331fc, 0xdc784f398623ac09};
#else
    uint64_t expected[2] = {0xfc317304b150a997, 0x09ac2386394f78dc};
#endif
    auto writer = this->newWriter(16);
    writer_write_uint64be(writer, 0xfc317304b150a997);
    writer_write_uint64be(writer, 0x09ac2386394f78dc);
    this->expectData(expected, 16);
}

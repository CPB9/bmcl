#include "bmcl/core/endian.h"
#include "bmcl/core/writer.h"
#include "bmcl/core/memwriter.h"
#include "bmcl/core/ringbuf.h"

#include "bmcl-test.h"

class Writer {
public:
    virtual ~Writer()
    {
    }

    bmcl_writer_t* get()
    {
        return _writer;
    }

    virtual std::size_t dataSize() const = 0;

    virtual void copyData(void* dest) const = 0;

protected:
    bmcl_writer_t* _writer;
};

class MemWriter : public Writer {
public:
    MemWriter(std::size_t size)
    {
        _memwriter = bmcl_memwriter_create(size);
        _writer = bmcl_memwriter_create_writer(_memwriter);
    }

    ~MemWriter()
    {
        bmcl_writer_destroy(_writer);
        bmcl_memwriter_destroy(_memwriter);
    }

    std::size_t dataSize() const
    {
        return bmcl_memwriter_size(_memwriter);
    }

    void copyData(void* dest) const
    {
        memcpy(dest, bmcl_memwriter_ptr(_memwriter), bmcl_memwriter_size(_memwriter));
    }

private:
    bmcl_memwriter_t* _memwriter;
};

class RingBufWriter : public Writer {
public:
    RingBufWriter(std::size_t size)
    {
        _ringbuf = bmcl_ringbuf_create(size);
        _writer = bmcl_ringbuf_create_writer(_ringbuf);
    }

    ~RingBufWriter()
    {
        bmcl_writer_destroy(_writer);
        bmcl_ringbuf_destroy(_ringbuf);
    }

    std::size_t dataSize() const
    {
        return bmcl_ringbuf_get_used_space(_ringbuf);
    }

    void copyData(void* dest) const
    {
        bmcl_ringbuf_peek(_ringbuf, dest, dataSize(), 0);
    }

private:
    bmcl_ringbuf_t* _ringbuf;
};

template <typename T>
class WriterTest : public ::testing::Test {
protected:
    template <std::size_t n, typename R>
    void newWriterWithSizeOf(const R (&array)[n])
    {
        (void)array;
        assert(_shell == 0);
        _shell = new T(sizeof(R) * n);
        _writer = _shell->get();
    }

    void SetUp()
    {
        _shell = 0;
    }

    template <std::size_t n, typename R>
    void expectData(const R (&array)[n])
    {
        std::size_t size = sizeof(R) * n;
        ASSERT_EQ(size, _shell->dataSize());
        R tmp[n];
        _shell->copyData(tmp);
        EXPECT_EQ_MEM(array, tmp, size);
    }

    template <std::size_t n, typename R>
    void writeData(const R (&array)[n])
    {
        bmcl_writer_write(_writer, array, sizeof(R) * n);
    }

    void writeUint8(uint8_t value)
    {
        bmcl_writer_write_uint8(_writer, value);
    }

    void writeUint16le(uint16_t value)
    {
        bmcl_writer_write_uint16le(_writer, value);
    }

    void writeUint32le(uint32_t value)
    {
        bmcl_writer_write_uint32le(_writer, value);
    }

    void writeUint64le(uint64_t value)
    {
        bmcl_writer_write_uint64le(_writer, value);
    }

    void writeUint16be(uint16_t value)
    {
        bmcl_writer_write_uint16be(_writer, value);
    }

    void writeUint32be(uint32_t value)
    {
        bmcl_writer_write_uint32be(_writer, value);
    }

    void writeUint64be(uint64_t value)
    {
        bmcl_writer_write_uint64be(_writer, value);
    }

    void TearDown()
    {
        if (_shell != 0) {
            delete _shell;
        }
    }

private:
    T* _shell;
    bmcl_writer_t* _writer;
};

typedef ::testing::Types<MemWriter, RingBufWriter> WriterTypes;
TYPED_TEST_CASE(WriterTest, WriterTypes);

TYPED_TEST(WriterTest, write)
{
    uint8_t expected[7] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77};
    uint8_t chunk1[4] = {0x11, 0x22, 0x33, 0x44};
    uint8_t chunk2[3] = {0x55, 0x66, 0x77};
    this->newWriterWithSizeOf(expected);
    this->writeData(chunk1);
    this->writeData(chunk2);
    this->expectData(expected);
}

TYPED_TEST(WriterTest, write_uint8)
{
    uint8_t expected[5] = {0xab, 0x12, 0x53, 0x98, 0xfa};
    this->newWriterWithSizeOf(expected);
    this->writeUint8(0xab);
    this->writeUint8(0x12);
    this->writeUint8(0x53);
    this->writeUint8(0x98);
    this->writeUint8(0xfa);
    this->expectData(expected);
}

TYPED_TEST(WriterTest, write_uint16le)
{
#ifdef BMCL_LITTLE_ENDIAN
    uint16_t expected[4] = {0x1234, 0x5678, 0x90ab, 0xcdef};
#else
    uint16_t expected[4] = {0x3412, 0x7856, 0xab90, 0xefcd};
#endif
    this->newWriterWithSizeOf(expected);
    this->writeUint16le(0x1234);
    this->writeUint16le(0x5678);
    this->writeUint16le(0x90ab);
    this->writeUint16le(0xcdef);
    this->expectData(expected);
}

TYPED_TEST(WriterTest, write_uint32le)
{
#ifdef BMCL_LITTLE_ENDIAN
    uint32_t expected[4] = {0x12345678, 0x90abcdef, 0x10293847, 0x56473829};
#else
    uint32_t expected[4] = {0x78563412, 0xefcdab90, 0x47382910, 0x29384756};
#endif
    this->newWriterWithSizeOf(expected);
    this->writeUint32le(0x12345678);
    this->writeUint32le(0x90abcdef);
    this->writeUint32le(0x10293847);
    this->writeUint32le(0x56473829);
    this->expectData(expected);
}

TYPED_TEST(WriterTest, write_uint64le)
{
#ifdef BMCL_LITTLE_ENDIAN
    uint64_t expected[2] = {0x1234567890abcdef, 0x1029384756473829};
#else
    uint64_t expected[2] = {0xefcdab9078563412, 0x2938475647382910};
#endif
    this->newWriterWithSizeOf(expected);
    this->writeUint64le(0x1234567890abcdef);
    this->writeUint64le(0x1029384756473829);
    this->expectData(expected);
}

TYPED_TEST(WriterTest, write_uint16be)
{
#ifdef BMCL_LITTLE_ENDIAN
    uint16_t expected[3] = {0x9584, 0x1876, 0x0573};
#else
    uint16_t expected[3] = {0x8495, 0x7618, 0x7305};
#endif
    this->newWriterWithSizeOf(expected);
    this->writeUint16be(0x8495);
    this->writeUint16be(0x7618);
    this->writeUint16be(0x7305);
    this->expectData(expected);
}

TYPED_TEST(WriterTest, write_uint32be)
{
#ifdef BMCL_LITTLE_ENDIAN
    uint32_t expected[3] = {0x05846377, 0x07594412, 0x09876543};
#else
    uint32_t expected[3] = {0x77638405, 0x12445907, 0x43658709};
#endif
    this->newWriterWithSizeOf(expected);
    this->writeUint32be(0x77638405);
    this->writeUint32be(0x12445907);
    this->writeUint32be(0x43658709);
    this->expectData(expected);
}

TYPED_TEST(WriterTest, write_uint64be)
{
#ifdef BMCL_LITTLE_ENDIAN
    uint64_t expected[2] = {0x97a950b1047331fc, 0xdc784f398623ac09};
#else
    uint64_t expected[2] = {0xfc317304b150a997, 0x09ac2386394f78dc};
#endif
    this->newWriterWithSizeOf(expected);
    this->writeUint64be(0xfc317304b150a997);
    this->writeUint64be(0x09ac2386394f78dc);
    this->expectData(expected);
}

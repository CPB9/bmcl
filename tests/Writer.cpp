#include "bmcl/Endian.h"
#include "bmcl/Writer.h"
#include "bmcl/MemWriter.h"
#include "bmcl/RingBuffer.h"

#include "BmclTest.h"

using namespace bmcl;

class WriterShell {
public:
    virtual ~WriterShell() {}

    Writer* get() { return _writer; }

    virtual std::size_t dataSize() const = 0;

    virtual void copyData(void* dest) const = 0;

protected:
    Writer* _writer;
};

class MemWriterShell : public WriterShell {
public:
    MemWriterShell(std::size_t size)
    {
        _memwriter = new MemWriter(size);
        _writer = _memwriter;
    }

    ~MemWriterShell() { delete _writer; }

    std::size_t dataSize() const { return _memwriter->sizeUsed(); }

    void copyData(void* dest) const { std::memcpy(dest, _memwriter->start(), _memwriter->sizeUsed()); }

private:
    MemWriter* _memwriter;
};

class RingBufferWriterShell : public WriterShell {
public:
    RingBufferWriterShell(std::size_t size)
    {
        _ringbuf = new RingBuffer(size);
        _writer = _ringbuf;
    }

    ~RingBufferWriterShell() { delete _writer; }

    std::size_t dataSize() const { return _ringbuf->usedSpace(); }

    void copyData(void* dest) const { _ringbuf->peek(dest, dataSize(), 0); }

private:
    RingBuffer* _ringbuf;
};

template <typename T>
class WriterTest : public ::testing::Test {
protected:
    WriterTest()
        : _shell(0)
        , _writer(0)
    {
    }

    template <std::size_t n, typename R>
    void newWriterWithSizeOf(const R (&array)[n])
    {
        (void)array;
        assert(_shell == 0);
        _shell = new T(sizeof(R) * n);
        _writer = _shell->get();
    }

    void SetUp() { _shell = 0; }

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
        _writer->write(array, sizeof(R) * n);
    }

    void writeUint8(uint8_t value) { _writer->writeUint8(value); }

    void writeUint16le(uint16_t value) { _writer->writeUint16Le(value); }

    void writeUint32le(uint32_t value) { _writer->writeUint32Le(value); }

    void writeUint64le(uint64_t value) { _writer->writeUint64Le(value); }

    void writeFloat32le(float value) { _writer->writeFloat32Le(value); }

    void writeFloat64le(double value) { _writer->writeFloat64Le(value); }

    void writeUint16be(uint16_t value) { _writer->writeUint16Be(value); }

    void writeUint32be(uint32_t value) { _writer->writeUint32Be(value); }

    void writeUint64be(uint64_t value) { _writer->writeUint64Be(value); }

    void writeFloat32be(float value) { _writer->writeFloat32Be(value); }

    void writeFloat64be(double value) { _writer->writeFloat64Be(value); }

    void TearDown()
    {
        if (_shell != 0) {
            delete _shell;
        }
    }

private:
    T* _shell;
    Writer* _writer;
};

typedef ::testing::Types<MemWriterShell, RingBufferWriterShell> WriterTypes;
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

TYPED_TEST(WriterTest, write_float32le)
{
#ifdef BMCL_LITTLE_ENDIAN
    uint32_t expected[2] = {0x12345678, 0x90abcdef};
#else
    uint32_t expected[2] = {0x78563412, 0xefcdab90};
#endif
    this->newWriterWithSizeOf(expected);
    uint32_t value1 = 0x12345678;
    this->writeFloat32le(*(float*)&value1);
    uint32_t value2 = 0x90abcdef;
    this->writeFloat32le(*(float*)&value2);
    this->expectData(expected);
}

TYPED_TEST(WriterTest, write_float64le)
{
#ifdef BMCL_LITTLE_ENDIAN
    uint64_t expected[2] = {0x1234567890abcdef, 0x1029384756473829};
#else
    uint64_t expected[2] = {0xefcdab9078563412, 0x2938475647382910};
#endif
    this->newWriterWithSizeOf(expected);
    uint64_t value1 = 0x1234567890abcdef;
    this->writeFloat64le(*(double*)&value1);
    uint64_t value2 = 0x1029384756473829;
    this->writeFloat64le(*(double*)&value2);
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

TYPED_TEST(WriterTest, write_float32be)
{
#ifdef BMCL_LITTLE_ENDIAN
    uint32_t expected[2] = {0x78563412, 0xefcdab90};
#else
    uint32_t expected[2] = {0x12345678, 0x90abcdef};
#endif
    this->newWriterWithSizeOf(expected);
    uint32_t value1 = 0x12345678;
    this->writeFloat32be(*(float*)&value1);
    uint32_t value2 = 0x90abcdef;
    this->writeFloat32be(*(float*)&value2);
    this->expectData(expected);
}

TYPED_TEST(WriterTest, write_float64be)
{
#ifdef BMCL_LITTLE_ENDIAN
    uint64_t expected[2] = {0xefcdab9078563412, 0x2938475647382910};
#else
    uint64_t expected[2] = {0x1234567890abcdef, 0x1029384756473829};
#endif
    this->newWriterWithSizeOf(expected);
    uint64_t value1 = 0x1234567890abcdef;
    this->writeFloat64be(*(double*)&value1);
    uint64_t value2 = 0x1029384756473829;
    this->writeFloat64be(*(double*)&value2);
    this->expectData(expected);
}


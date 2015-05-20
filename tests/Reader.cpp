#include "bmcl/Endian.h"
#include "bmcl/MemReader.h"
#include "bmcl/Reader.h"
#include "bmcl/RingBuffer.h"

#include "BmclTest.h"

using namespace bmcl;

class ReaderShell {
public:
    virtual ~ReaderShell() {}

    Reader* get() { return _reader; }

protected:
    Reader* _reader;
};

class MemReaderShell : public ReaderShell {
public:
    MemReaderShell(const void* ptr, std::size_t size) { _reader = new MemReader(ptr, size); }

    ~MemReaderShell() { delete _reader; }
};

class RingBufferReaderShell : public ReaderShell {
public:
    RingBufferReaderShell(const void* ptr, std::size_t size)
    {
        _ringbuf = new RingBuffer(size);
        std::size_t fillerSize = size / 2 + 1;
        uint8_t* filler = new uint8_t[fillerSize];
        _ringbuf->write(filler, fillerSize);
        _ringbuf->write(ptr, size);
        _reader = _ringbuf;
        delete[] filler;
    }

    ~RingBufferReaderShell() { delete _reader; }

private:
    RingBuffer* _ringbuf;
};

template <typename T>
class ReaderTest : public ::testing::Test {
protected:
    ReaderTest()
        : _shell(0)
        , _reader(0)
    {
    }

    template <std::size_t n, typename R>
    void newReader(const R (&array)[n])
    {
        assert(_shell == 0);
        _shell = new T(array, sizeof(R) * n);
        _reader = _shell->get();
    }

    void SetUp() { _shell = 0; }

    template <std::size_t n, typename R>
    void expectNextData(const R (&array)[n])
    {
        std::size_t size = sizeof(R) * n;
        R tmp[n];
        _reader->read(tmp, size);
        EXPECT_EQ_MEM(array, tmp, size);
    }

    void expectNextUint8(uint8_t value) { EXPECT_EQ(value, _reader->readUint8()); }

    void expectNextUint16be(uint16_t value) { EXPECT_EQ(value, _reader->readUint16Be()); }

    void expectNextUint32be(uint32_t value) { EXPECT_EQ(value, _reader->readUint32Be()); }

    void expectNextUint64be(uint64_t value) { EXPECT_EQ(value, _reader->readUint64Be()); }

    void expectNextUint16le(uint16_t value) { EXPECT_EQ(value, _reader->readUint16Le()); }

    void expectNextUint32le(uint32_t value) { EXPECT_EQ(value, _reader->readUint32Le()); }

    void expectNextUint64le(uint64_t value) { EXPECT_EQ(value, _reader->readUint64Le()); }

    void expectNextFloat32le(float value) { EXPECT_EQ(value, _reader->readFloat32Le()); }

    void expectNextFloat64le(double value) { EXPECT_EQ(value, _reader->readFloat64Le()); }

    void expectNextFloat32be(float value) { EXPECT_EQ(value, _reader->readFloat32Be()); }

    void expectNextFloat64be(double value) { EXPECT_EQ(value, _reader->readFloat64Be()); }

    void TearDown()
    {
        if (_shell != 0) {
            delete _shell;
        }
    }

private:
    T* _shell;
    Reader* _reader;
};

typedef ::testing::Types<MemReaderShell, RingBufferReaderShell> ReaderTypes;
TYPED_TEST_CASE(ReaderTest, ReaderTypes);

TYPED_TEST(ReaderTest, read)
{
    uint8_t data[10] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x00};
    uint8_t expected1[3] = {0x11, 0x22, 0x33};
    uint8_t expected2[2] = {0x44, 0x55};
    uint8_t expected3[5] = {0x66, 0x77, 0x88, 0x99, 0x00};
    this->newReader(data);
    this->expectNextData(expected1);
    this->expectNextData(expected2);
    this->expectNextData(expected3);
}

TYPED_TEST(ReaderTest, read_uint8)
{
    uint8_t data[5] = {0xaa, 0x72, 0x13, 0x42, 0x25};
    this->newReader(data);
    this->expectNextUint8(0xaa);
    this->expectNextUint8(0x72);
    this->expectNextUint8(0x13);
    this->expectNextUint8(0x42);
    this->expectNextUint8(0x25);
}

TYPED_TEST(ReaderTest, read_uint16le)
{
#ifdef BMCL_LITTLE_ENDIAN
    uint16_t data[4] = {0x1234, 0x5678, 0xaabb, 0xccdd};
#else
    uint16_t data[4] = {0x3412, 0x7856, 0xbbaa, 0xddcc};
#endif
    this->newReader(data);
    this->expectNextUint16le(0x1234);
    this->expectNextUint16le(0x5678);
    this->expectNextUint16le(0xaabb);
    this->expectNextUint16le(0xccdd);
}

TYPED_TEST(ReaderTest, read_uint32le)
{
#ifdef BMCL_LITTLE_ENDIAN
    uint32_t data[4] = {0x12345678, 0xabcdef12, 0xaabbccdd, 0xff112233};
#else
    uint32_t data[4] = {0x78563412, 0x12efcdab, 0xddccbbaa, 0x332211ff};
#endif
    this->newReader(data);
    this->expectNextUint32le(0x12345678);
    this->expectNextUint32le(0xabcdef12);
    this->expectNextUint32le(0xaabbccdd);
    this->expectNextUint32le(0xff112233);
}

TYPED_TEST(ReaderTest, read_uint64le)
{
#ifdef BMCL_LITTLE_ENDIAN
    uint64_t data[4] = {0x1122334455667788, 0x3344556677889900, 0xaabbccddeeff0099, 0xaa11bb22cc33dd44};
#else
    uint64_t data[4] = {0x8877665544332211, 0x0099887766554433, 0x9900ffeeddccbbaa, 0x44dd33cc22bb11aa};
#endif
    this->newReader(data);
    this->expectNextUint64le(0x1122334455667788);
    this->expectNextUint64le(0x3344556677889900);
    this->expectNextUint64le(0xaabbccddeeff0099);
    this->expectNextUint64le(0xaa11bb22cc33dd44);
}

TYPED_TEST(ReaderTest, read_float32le)
{
#ifdef BMCL_LITTLE_ENDIAN
    uint32_t data[2] = {0x12345678, 0xabcdef12};
#else
    uint32_t data[2] = {0x78563412, 0x12efcdab};
#endif
    this->newReader(data);
    uint32_t value1 = 0x12345678;
    this->expectNextFloat32le(*(float*)&value1);
    uint32_t value2 = 0xabcdef12;
    this->expectNextFloat32le(*(float*)&value2);
}

TYPED_TEST(ReaderTest, read_float64le)
{
#ifdef BMCL_LITTLE_ENDIAN
    uint64_t data[2] = {0x1122334455667788, 0x3344556677889900};
#else
    uint64_t data[2] = {0x8877665544332211, 0x0099887766554433};
#endif
    this->newReader(data);
    uint64_t value1 = 0x1122334455667788;
    this->expectNextFloat64le(*(double*)&value1);
    uint64_t value2 = 0x3344556677889900;
    this->expectNextFloat64le(*(double*)&value2);
}

TYPED_TEST(ReaderTest, read_uint16be)
{
#ifdef BMCL_LITTLE_ENDIAN
    uint16_t data[6] = {0xaabb, 0x1122, 0x4455, 0x7766, 0x8899, 0xffcc};
#else
    uint16_t data[6] = {0xbbaa, 0x2211, 0x5544, 0x6677, 0x9988, 0xccff};
#endif
    this->newReader(data);
    this->expectNextUint16be(0xbbaa);
    this->expectNextUint16be(0x2211);
    this->expectNextUint16be(0x5544);
    this->expectNextUint16be(0x6677);
    this->expectNextUint16be(0x9988);
    this->expectNextUint16be(0xccff);
}

TYPED_TEST(ReaderTest, read_uint32be)
{
#ifdef BMCL_LITTLE_ENDIAN
    uint32_t data[3] = {0x77665544, 0xbbdd0099, 0x6600aaff};
#else
    uint32_t data[3] = {0x44556677, 0x9900ddbb, 0xffaa0066};
#endif
    this->newReader(data);
    this->expectNextUint32be(0x44556677);
    this->expectNextUint32be(0x9900ddbb);
    this->expectNextUint32be(0xffaa0066);
}

TYPED_TEST(ReaderTest, read_uint64be)
{
#ifdef BMCL_LITTLE_ENDIAN
    uint64_t data[2] = {0x0088665533119977, 0xaaccee2244ffbbdd};
#else
    uint64_t data[2] = {0x7799113355668800, 0xddbbff4422eeccaa};
#endif
    this->newReader(data);
    this->expectNextUint64be(0x7799113355668800);
    this->expectNextUint64be(0xddbbff4422eeccaa);
}

TYPED_TEST(ReaderTest, read_float32be)
{
#ifdef BMCL_LITTLE_ENDIAN
    uint32_t data[2] = {0x78563412, 0x12efcdab};
#else
    uint32_t data[2] = {0x12345678, 0xabcdef12};
#endif
    this->newReader(data);
    uint32_t value1 = 0x12345678;
    this->expectNextFloat32be(*(float*)&value1);
    uint32_t value2 = 0xabcdef12;
    this->expectNextFloat32be(*(float*)&value2);
}

TYPED_TEST(ReaderTest, read_float64be)
{
#ifdef BMCL_LITTLE_ENDIAN
    uint64_t data[2] = {0x8877665544332211, 0x0099887766554433};
#else
    uint64_t data[2] = {0x1122334455667788, 0x3344556677889900};
#endif
    this->newReader(data);
    uint64_t value1 = 0x1122334455667788;
    this->expectNextFloat64be(*(double*)&value1);
    uint64_t value2 = 0x3344556677889900;
    this->expectNextFloat64be(*(double*)&value2);
}

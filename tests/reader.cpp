#include "bmcl/core/endian.h"
#include "bmcl/core/memreader.h"
#include "bmcl/core/reader.h"
#include "bmcl/core/ringbuf.h"

#include "bmcl-test.h"

class Reader {
public:
    virtual ~Reader()
    {
    }

    bmcl_reader_t* get()
    {
        return _reader;
    }

protected:
    bmcl_reader_t* _reader;
};

class MemReader : public Reader {
public:
    MemReader(const void* ptr, std::size_t size)
    {
        _memreader = bmcl_memreader_create(ptr, size);
        _reader = bmcl_memreader_create_reader(_memreader);
    }

    ~MemReader()
    {
        bmcl_reader_destroy(_reader);
        bmcl_memreader_destroy(_memreader);
    }

private:
    bmcl_memreader_t* _memreader;
};

class RingBufReader : public Reader {
public:
    RingBufReader(const void* ptr, std::size_t size)
    {
        _ringbuf = bmcl_ringbuf_create(size);
        std::size_t fillerSize = size / 2 + 1;
        uint8_t* filler = new uint8_t[fillerSize];
        bmcl_ringbuf_write(_ringbuf, filler, fillerSize);
        bmcl_ringbuf_write(_ringbuf, ptr, size);
        _reader = bmcl_ringbuf_create_reader(_ringbuf);
        delete[] filler;
    }

    ~RingBufReader()
    {
        bmcl_reader_destroy(_reader);
        bmcl_ringbuf_destroy(_ringbuf);
    }

private:
    bmcl_ringbuf_t* _ringbuf;
};

template <typename T>
class ReaderTest : public ::testing::Test {
protected:
    template <std::size_t n, typename R>
    void newReader(const R (&array)[n])
    {
        assert(_shell == 0);
        _shell = new T(array, sizeof(R) * n);
        _reader = _shell->get();
    }

    void SetUp()
    {
        _shell = 0;
    }

    template <std::size_t n, typename R>
    void expectNextData(const R (&array)[n])
    {
        std::size_t size = sizeof(R) * n;
        uint8_t tmp[size];
        bmcl_reader_read(_reader, tmp, size);
        EXPECT_EQ_MEM(array, tmp, size);
    }

    template <typename R, typename F>
    void expectNext(R data, F func)
    {
        R tmp;
        tmp = func(_reader);
        EXPECT_EQ(data, tmp);
    }

    void expectNextUint8(uint8_t value)
    {
        expectNext(value, bmcl_reader_read_uint8);
    }

    void expectNextUint16be(uint16_t value)
    {
        expectNext(value, bmcl_reader_read_uint16be);
    }

    void expectNextUint32be(uint32_t value)
    {
        expectNext(value, bmcl_reader_read_uint32be);
    }

    void expectNextUint64be(uint64_t value)
    {
        expectNext(value, bmcl_reader_read_uint64be);
    }

    void expectNextUint16le(uint16_t value)
    {
        expectNext(value, bmcl_reader_read_uint16le);
    }

    void expectNextUint32le(uint32_t value)
    {
        expectNext(value, bmcl_reader_read_uint32le);
    }

    void expectNextUint64le(uint64_t value)
    {
        expectNext(value, bmcl_reader_read_uint64le);
    }

    void TearDown()
    {
        if (_shell != 0) {
            delete _shell;
        }
    }

private:
    T* _shell;
    bmcl_reader_t* _reader;
};

typedef ::testing::Types<MemReader, RingBufReader> ReaderTypes;
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

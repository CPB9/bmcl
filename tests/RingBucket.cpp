#include "bmcl/RingBucket.h"

#include "BmclTest.h"

using namespace bmcl;

class RingBucketTest : public ::testing::Test {
protected:
    RingBucketTest()
        : _ringbucket(0)
    {
    }

    void SetUp() { _ringbucket = 0; }

    void TearDown()
    {
        if (_ringbucket) {
            delete _ringbucket;
        }
    }

    void initRingBucket(std::size_t size)
    {
        assert(_ringbucket == 0);
        _ringbucket = new RingBucket(size);
    }

    template <std::size_t n, typename R>
    void appendElement(const R (&array)[n])
    {
        _ringbucket->append(array, sizeof(R) * n);
    }

    template <std::size_t n, typename R>
    void expectNextElement(const R (&array)[n])
    {
        std::size_t elementSize = sizeof(R) * n;
        ASSERT_EQ(elementSize, _ringbucket->firstSize());
        R tmp[n];
        _ringbucket->copyFirst(tmp);
        _ringbucket->removeFirst();
        EXPECT_EQ_MEM(array, tmp, elementSize);
    }

    void expectCount(std::size_t count) { EXPECT_EQ(count, _ringbucket->count()); }

    void expectFreeSpace(std::size_t size) { EXPECT_EQ(size, _ringbucket->freeSpace()); }

    void expectEmpty(bool isEmpty = true) { EXPECT_EQ(isEmpty, _ringbucket->isEmpty()); }

    void expectFirstElementSize(std::size_t size) { EXPECT_EQ(size, _ringbucket->firstSize()); }

    std::size_t fullSize(std::size_t elementSize) { return elementSize + RingBucket::headerSize(); }

    void reset() { _ringbucket->reset(); }

private:
    RingBucket* _ringbucket;
};

TEST_F(RingBucketTest, init)
{
    initRingBucket(100);
    expectCount(0);
    expectFreeSpace(100);
    expectEmpty();
}

TEST_F(RingBucketTest, reset)
{
    uint8_t element[5] = {0xaa, 0xbb, 0xcc, 0xdd, 0xff};
    initRingBucket(100);
    appendElement(element);
    reset();
    expectCount(0);
    expectFreeSpace(100);
    expectEmpty();
}

TEST_F(RingBucketTest, append_one)
{
    uint8_t element[5] = {0xaa, 0xbb, 0xcc, 0xdd, 0xff};
    uint8_t expected[5] = {0xaa, 0xbb, 0xcc, 0xdd, 0xff};
    initRingBucket(100);
    appendElement(element);
    expectCount(1);
    expectFreeSpace(100 - fullSize(5));
    expectFirstElementSize(5);
    expectEmpty(false);
    expectNextElement(expected);
    expectCount(0);
    expectFreeSpace(100);
    expectEmpty();
}

TEST_F(RingBucketTest, append_several)
{
    uint8_t element1[5] = {0xaa, 0xbb, 0xcc, 0xdd, 0xff};
    uint8_t element2[3] = {0x44, 0x22, 0x00};
    uint8_t element3[4] = {0x11, 0x55, 0x66, 0x77};
    initRingBucket(100);
    appendElement(element1);
    appendElement(element2);
    appendElement(element3);
    expectNextElement(element1);
    expectNextElement(element2);
    expectNextElement(element3);
}

TEST_F(RingBucketTest, overwrite)
{
    uint8_t element1[5] = {0xaa, 0xbb, 0xcc, 0xdd, 0xff};
    uint8_t element2[3] = {0x44, 0x22, 0x00};
    uint8_t element3[4] = {0x11, 0x55, 0x66, 0x77};
    uint8_t element4[5] = {0xff, 0xcc, 0xdb, 0xbd};
    initRingBucket(fullSize(5) + fullSize(3) + fullSize(4));
    appendElement(element1);
    appendElement(element2);
    appendElement(element3);
    expectFreeSpace(0);
    appendElement(element4);
    expectFreeSpace(0);
    expectNextElement(element2);
    expectNextElement(element3);
    expectNextElement(element4);
}

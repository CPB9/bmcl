#include "bmcl/core/ringbucket.h"

#include "bmcl-test.h"

class RingBucketTest : public ::testing::Test {
protected:
    void SetUp()
    {
        _ringbucket = 0;
    }

    void TearDown()
    {
        if (_ringbucket) {
            bmcl_ringbucket_destroy(_ringbucket);
        }
    }

    void initRingBucket(std::size_t size)
    {
        assert(_ringbucket == 0);
        _ringbucket = bmcl_ringbucket_create(size);
    }

    template <std::size_t n, typename R>
    void appendElement(const R (&array)[n])
    {
        bmcl_ringbucket_append(_ringbucket, array, sizeof(R) * n);
    }

    template <std::size_t n, typename R>
    void expectNextElement(const R (&array)[n])
    {
        std::size_t elementSize = sizeof(R) * n;
        ASSERT_EQ(elementSize, bmcl_ringbucket_first_size(_ringbucket));
        R tmp[n];
        bmcl_ringbucket_copy_first(_ringbucket, tmp);
        bmcl_ringbucket_remove_first(_ringbucket);
        EXPECT_EQ_MEM(array, tmp, elementSize);
    }

    void expectCount(std::size_t count)
    {
        EXPECT_EQ(count, bmcl_ringbucket_count(_ringbucket));
    }

    void expectFreeSpace(std::size_t size)
    {
        EXPECT_EQ(size, bmcl_ringbucket_get_free_space(_ringbucket));
    }

    void expectEmpty(bool isEmpty = true)
    {
        EXPECT_EQ(isEmpty, bmcl_ringbucket_is_empty(_ringbucket));
    }

    void expectFirstElementSize(std::size_t size)
    {
        EXPECT_EQ(size, bmcl_ringbucket_first_size(_ringbucket));
    }

    std::size_t fullSize(std::size_t elementSize)
    {
        return elementSize + bmcl_ringbucket_header_size();
    }

    void reset()
    {
        bmcl_ringbucket_reset(_ringbucket);
    }

private:
    bmcl_ringbucket_t* _ringbucket;
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

TEST_F(RingBucketTest, queue_el_size)
{
    bmcl_ringbucket_t* bucket = bmcl_ringbucket_create(100);
    bmcl_queue_t* queue = bmcl_ringbucket_create_queue(bucket);
    EXPECT_FALSE(bmcl_queue_const_el_size(queue, 0));
    bmcl_queue_destroy(queue);
    bmcl_ringbucket_destroy(bucket);
}

#include "bmcl/core/ringarray.h"

#include "bmcl-test.h"

class RingArrayTest : public ::testing::Test {
protected:
    void SetUp()
    {
        _ringarray = 0;
    }

    void TearDown()
    {
        if (_ringarray) {
            ringarray_destroy(_ringarray);
        }
    }

    void initRingArray(std::size_t elementNum, std::size_t elementSize)
    {
        assert(_ringarray == 0);
        _ringarray = ringarray_create(elementNum, elementSize);
    }

    template <std::size_t n, typename R>
    void appendElement(const R (&array)[n])
    {
        std::size_t elementSize = sizeof(R) * n;
        assert(elementSize == ringarray_element_size(_ringarray));
        ringarray_append(_ringarray, array);
    }

    template <std::size_t n, typename R>
    void expectNextElement(const R (&array)[n])
    {
        std::size_t elementSize = sizeof(R) * n;
        assert(elementSize == ringarray_element_size(_ringarray));
        R tmp[n];
        ringarray_copy_first(_ringarray, tmp);
        ringarray_remove_first(_ringarray);
        EXPECT_EQ_MEM(array, tmp, elementSize);
    }

    void expectEmpty(bool isEmpty = true)
    {
        EXPECT_EQ(isEmpty, ringarray_is_empty(_ringarray));
    }

    void expectFull(bool isFull = true)
    {
        EXPECT_EQ(isFull, ringarray_is_full(_ringarray));
    }

    void expectCount(std::size_t count)
    {
        EXPECT_EQ(count, ringarray_count(_ringarray));
    }

    void expectSize(std::size_t size)
    {
        EXPECT_EQ(size, ringarray_size(_ringarray));
    }

    void expectElementSize(std::size_t size)
    {
        EXPECT_EQ(size, ringarray_element_size(_ringarray));
    }

private:
    ringarray_t* _ringarray;
};

TEST_F(RingArrayTest, init)
{
    initRingArray(10, 5);
    expectEmpty();
    expectFull(false);
    expectCount(0);
    expectElementSize(5);
    expectSize(10);
}

TEST_F(RingArrayTest, append_one)
{
    uint8_t element[5] = {0xaa, 0x44, 0x55, 0x66, 0x11};
    uint8_t expected[5] = {0xaa, 0x44, 0x55, 0x66, 0x11};
    initRingArray(5, 5);
    appendElement(element);
    expectCount(1);
    expectEmpty(false);
    expectFull(false);
    expectNextElement(expected);
    expectCount(0);
    expectEmpty();
    expectFull(false);
}

TEST_F(RingArrayTest, append_several)
{
    uint8_t element1[4] = {0x77, 0x44, 0x99, 0x66};
    uint8_t element2[4] = {0x2a, 0x44, 0x55, 0x00};
    uint8_t element3[4] = {0x44, 0x33, 0xaa, 0xff};
    initRingArray(3, 4);
    expectCount(0);
    expectEmpty();
    expectFull(false);
    appendElement(element1);
    expectCount(1);
    expectEmpty(false);
    expectFull(false);
    appendElement(element2);
    expectCount(2);
    expectEmpty(false);
    expectFull(false);
    appendElement(element3);
    expectCount(3);
    expectEmpty(false);
    expectFull();
    expectNextElement(element1);
    expectCount(2);
    expectEmpty(false);
    expectFull(false);
    expectNextElement(element2);
    expectCount(1);
    expectEmpty(false);
    expectFull(false);
    expectNextElement(element3);
    expectCount(0);
    expectEmpty();
    expectFull(false);
}

TEST_F(RingArrayTest, overwrite_one)
{
    uint8_t element1[7] = {0x12, 0x45, 0x85, 0x88, 0x99, 0x44, 0x33};
    uint8_t element2[7] = {0x45, 0x14, 0xfc, 0x11, 0x00, 0x51, 0x93};
    uint8_t element3[7] = {0x01, 0x88, 0x44, 0x78, 0x22, 0x11, 0x0b};
    initRingArray(2, 7);
    appendElement(element1);
    appendElement(element2);
    appendElement(element3);
    expectCount(2);
    expectEmpty(false);
    expectFull();
    expectNextElement(element2);
    expectNextElement(element3);
}

TEST_F(RingArrayTest, queue_el_size)
{
    ringarray_t* array = ringarray_create(100, 17);
    queue_t* queue = ringarray_create_queue(array);
    std::size_t size = 0;
    EXPECT_TRUE(queue_const_el_size(queue, &size));
    EXPECT_EQ(size, 17);
    queue_destroy(queue);
    ringarray_destroy(array);
}

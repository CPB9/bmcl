#include "bmcl/RingArray.h"

#include "BmclTest.h"

using namespace bmcl;

class RingArrayTest : public ::testing::Test {
protected:
    RingArrayTest()
        : _ringarray(0)
    {
    }

    void SetUp() { _ringarray = 0; }

    void TearDown()
    {
        if (_ringarray) {
            delete _ringarray;
        }
    }

    void initRingArray(std::size_t elementNum, std::size_t elementSize)
    {
        assert(_ringarray == 0);
        _ringarray = new RingArray(elementNum, elementSize);
    }

    template <std::size_t n, typename R>
    void appendElement(const R (&array)[n])
    {
        assert(sizeof(R) * n == _ringarray->elementSize());
        _ringarray->append(array);
    }

    template <std::size_t n, typename R>
    void expectNextElement(const R (&array)[n])
    {
        std::size_t elementSize = sizeof(R) * n;
        assert(elementSize == _ringarray->elementSize());
        R tmp[n];
        _ringarray->copyFirst(tmp);
        _ringarray->removeFirst();
        EXPECT_EQ_MEM(array, tmp, elementSize);
    }

    void expectEmpty(bool isEmpty = true) { EXPECT_EQ(isEmpty, _ringarray->isEmpty()); }

    void expectFull(bool isFull = true) { EXPECT_EQ(isFull, _ringarray->isFull()); }

    void expectCount(std::size_t count) { EXPECT_EQ(count, _ringarray->count()); }

    void expectSize(std::size_t size) { EXPECT_EQ(size, _ringarray->size()); }

    void expectElementSize(std::size_t size) { EXPECT_EQ(size, _ringarray->elementSize()); }

private:
    RingArray* _ringarray;
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

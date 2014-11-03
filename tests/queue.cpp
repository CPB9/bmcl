#include "bmcl/core/ringarray.h"
#include "bmcl/core/ringbucket.h"
#include "bmcl/core/queue.h"

#include "bmcl-test.h"

class Queue {
public:
    virtual ~Queue()
    {
    }

    queue_t* get()
    {
        return _queue;
    }

protected:
    queue_t* _queue;
};

class RingArrayQueue : public Queue {
public:
    RingArrayQueue(std::size_t elementNum, std::size_t elementSize)
    {
        _ringarray = ringarray_create(elementNum, elementSize);
        _queue = ringarray_create_queue(_ringarray);
    }

    ~RingArrayQueue()
    {
        queue_destroy(_queue);
        ringarray_destroy(_ringarray);
    }

private:
    ringarray_t* _ringarray;
};

class RingBucketQueue : public Queue {
public:
    RingBucketQueue(std::size_t elementNum, std::size_t elementSize)
    {
        _ringbucket = ringbucket_create(elementNum * elementSize);
        _queue = ringbucket_create_queue(_ringbucket);
    }

    ~RingBucketQueue()
    {
        queue_destroy(_queue);
        ringbucket_destroy(_ringbucket);
    }

private:
    ringbucket_t* _ringbucket;
};

template <typename T>
class QueueTest : public ::testing::Test {
protected:
    void newQueue(std::size_t minElementNum, std::size_t minElementSize)
    {
        assert(_shell == 0);
        _shell = new T(minElementNum, minElementSize);
        _queue = _shell->get();
    }

    void SetUp()
    {
        _shell = 0;
    }

    void TearDown()
    {
        if (_shell != 0) {
            delete _shell;
        }
    }

    template <std::size_t n, typename R>
    void append(const R (&array)[n])
    {
        queue_append(_queue, array, sizeof(R) * n);
    }

    template <std::size_t n, typename R>
    void expectNextElement(const R (&array)[n])
    {
        std::size_t size = sizeof(R) * n;
        ASSERT_EQ(size, queue_first_size(_queue));
        R tmp[n];
        queue_copy_first(_queue, tmp);
        queue_remove_first(_queue);
        EXPECT_EQ_MEM(array, tmp, size);
    }

    void expectEmpty(bool isEmpty = true)
    {
        EXPECT_EQ(isEmpty, queue_is_empty(_queue));
    }

    void expectCount(std::size_t count)
    {
        EXPECT_EQ(count, queue_count(_queue));
    }

private:
    T* _shell;
    queue_t* _queue;
};

typedef ::testing::Types<RingArrayQueue, RingBucketQueue> QueueTypes;
TYPED_TEST_CASE(QueueTest, QueueTypes);

TYPED_TEST(QueueTest, init)
{
    this->newQueue(4, 2);
    this->expectCount(0);
    this->expectEmpty();
}

TYPED_TEST(QueueTest, append_several)
{
    uint8_t element1[5] = {0xaa, 0xbb, 0x11, 0x22, 0x99};
    uint8_t element2[5] = {0x33, 0x44, 0x77, 0xff, 0xcc};
    this->newQueue(10, 5);
    this->append(element1);
    this->expectEmpty(false);
    this->expectCount(1);
    this->append(element2);
    this->expectEmpty(false);
    this->expectCount(2);
    this->expectNextElement(element1);
    this->expectNextElement(element2);
}

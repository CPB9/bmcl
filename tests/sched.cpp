#include "bmcl/core/sched.h"

#include "bmcl-test.h"

class SchedTest : public ::testing::Test {
protected:
    void SetUp()
    {
    }

    void TearDown()
    {
    }
};

struct UserData {
    uint64_t maxCount;
    uint64_t execCount;
    std::size_t counters[4];
};

static bool executor1(void* fp, void* userData)
{
    UserData* data = (UserData*)userData;
    void (*func)(std::size_t*) = (void (*)(std::size_t*))fp;
    func(data->counters);
    data->execCount++;
    return data->execCount < data->maxCount;
}

static void inc1(std::size_t* counter)
{
    counter[0] += 1;
}

static void inc2(std::size_t* counter)
{
    counter[1] += 1;
}

static void inc3(std::size_t* counter)
{
    counter[2] += 1;
}

static void inc4(std::size_t* counter)
{
    counter[3] += 1;
}

TEST_F(SchedTest, test1)
{
    bmcl_sched_t* sched;
    const task_t tasks[4] = {{(void*)inc1, 3}, {(void*)inc2, 11}, {(void*)inc3, 101}, {(void*)inc4, 211}};
    sched = bmcl_sched_create(tasks, 4, executor1);
    uint64_t maxCount = 11 * 101 * 211 + 3 * 101 * 211 + 3 * 11 * 211 + 3 * 11 * 101;
    UserData userData = {maxCount, 0, {0, 0, 0, 0}};
    bmcl_sched_exec_while_possible(sched, &userData);
    EXPECT_EQ(11 * 101 * 211, userData.counters[0]);
    EXPECT_EQ(3 * 101 * 211, userData.counters[1]);
    EXPECT_EQ(3 * 11 * 211, userData.counters[2]);
    EXPECT_EQ(3 * 11 * 101, userData.counters[3]);
    bmcl_sched_destroy(sched);
}

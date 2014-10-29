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
    uint64_t max_count;
    uint64_t exec_count;
    size_t counters[4];
};

static bool executor1(void* fp, void* userData)
{
    UserData* data = (UserData*)userData;
    void (*func)(size_t*) = (void (*)(size_t*))fp;
    func(data->counters);
    data->exec_count++;
    return data->exec_count < data->max_count;
}

static void inc1(size_t* counter)
{
    counter[0] += 1;
}

static void inc2(size_t* counter)
{
    counter[1] += 1;
}

static void inc3(size_t* counter)
{
    counter[2] += 1;
}

static void inc4(size_t* counter)
{
    counter[3] += 1;
}

TEST_F(SchedTest, test1)
{
    sched_t* sched;
    const task_t tasks[4] = {{(void*)inc1, 3}, {(void*)inc2, 11}, {(void*)inc3, 101}, {(void*)inc4, 211}};
    sched = sched_create(tasks, 4, executor1);
    uint64_t max_count = 11 * 101 * 211 + 3 * 101 * 211 + 3 * 11 * 211 + 3 * 11 * 101;
    UserData userData = {max_count, 0, {0, 0, 0, 0}};
    sched_exec_while_possible(sched, &userData);
    EXPECT_EQ(11 * 101 * 211, userData.counters[0]);
    EXPECT_EQ(3 * 101 * 211, userData.counters[1]);
    EXPECT_EQ(3 * 11 * 211, userData.counters[2]);
    EXPECT_EQ(3 * 11 * 101, userData.counters[3]);
    sched_destroy(sched);
}

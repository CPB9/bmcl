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

static bool executor1(void* fp, void* userData)
{
    size_t* data = (size_t*)userData;
    void (*func)(size_t*) = (void (*)(size_t*))fp;
    func(data);
    return true;
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
    sched_t sched;
    const task_t tasks[4] = {{(void*)inc1, 3}, {(void*)inc2, 11}, {(void*)inc3, 101}, {(void*)inc4, 211}};
    task_data_t taskData[4];
    sched_init(&sched, tasks, taskData, 4, executor1);
    size_t userData[4] = {0, 0, 0, 0};
    for (int i = 0; i < 11 * 101 * 211 + 3 * 101 * 211 + 3 * 11 * 211 + 3 * 11 * 101; i++) {
        sched_exec_next(&sched, userData);
    }
    EXPECT_EQ(11 * 101 * 211, userData[0]);
    EXPECT_EQ(3 * 101 * 211, userData[1]);
    EXPECT_EQ(3 * 11 * 211, userData[2]);
    EXPECT_EQ(3 * 11 * 101, userData[3]);
}

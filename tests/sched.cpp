#include "bmcl/sched.h"

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
    const task_t tasks[4] = {{(void*)inc1, 3}, {(void*)inc2, 11}, {(void*)inc3, 101}, {(void*)inc4, 202}};
    task_data_t taskData[4];
    sched_init(&sched, tasks, taskData, 4, executor1);
    size_t userData[4] = {0, 0, 0, 0};
    for (int i = 0; i < 295627; i++) {
        sched_exec_next(&sched, userData);
    }
//     EXPECT_EQ(5, userData[0]);
//     EXPECT_EQ(2, userData[1]);
//     EXPECT_EQ(1, userData[2]);
    std::cout << userData[0] << std::endl;
    std::cout << userData[1] << std::endl;
    std::cout << userData[2] << std::endl;
    std::cout << userData[3] << std::endl;
    std::cout << taskData[0].weight << std::endl;
    std::cout << taskData[1].weight << std::endl;
    std::cout << taskData[2].weight << std::endl;
    std::cout << taskData[3].weight << std::endl;
}

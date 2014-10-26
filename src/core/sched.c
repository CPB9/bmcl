#include "bmcl/core/sched.h"

#include <assert.h>
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void sched_init(sched_t* self, const task_t* tasks, task_data_t* task_data, size_t task_num, task_executor_t executor)
{
    assert(task_num > 0);
    assert(task_num <= UINT_MAX / sizeof(task_data_t));
    for (size_t i = 0; i < task_num; i++) {
        task_data[i].is_enabled = true;
        task_data[i].weight = 0;
    }
    self->tasks = tasks;
    self->task_data = task_data;
    self->task_num = task_num;
    self->executor = executor;
}

#if BMCL_HAVE_MALLOC

sched_t* sched_create(const task_t* tasks, size_t task_num, task_executor_t executor)
{
    uint8_t* data = malloc(sizeof(sched_t) + sizeof(task_data_t) * task_num);
    sched_t* self = (sched_t*)data;
    sched_init(self, tasks, (task_data_t*)(data + sizeof(sched_t)), task_num, executor);
    return self;
}

void sched_destroy(sched_t* self)
{
    free(self);
}

#endif

bool sched_exec_next(sched_t* self, void* user_data)
{
    size_t task_index = 0;
    int max_weight = self->task_data[0].weight;
    self->task_data[0].weight += 1;
    for (size_t i = 1; i < self->task_num; i++) {
        int weight = self->task_data[i].weight;
        self->task_data[i].weight += 1;
        if (weight > max_weight) {
            max_weight = weight;
            task_index = i;
        }
    }
    const task_t* task = &self->tasks[task_index];
    self->task_data[task_index].weight -= task->priority;
    return self->executor(task->task, user_data);
}

void sched_exec_while_possible(sched_t* self, void* user_data)
{
    while (sched_exec_next(self, user_data))
        ;
}

#pragma once

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif
/* O(n) */

typedef bool (*task_executor_t)(void* task, void* user_data);

typedef struct {
    void* task;
    size_t priority;
} task_t;

typedef struct {
    int weight;
    bool is_enabled;
} task_data_t;

typedef struct {
    const task_t* tasks;
    task_data_t* task_data;
    size_t task_num;
    task_executor_t executor;
} sched_t;

void sched_init(sched_t* self, const task_t* tasks, task_data_t* task_data, size_t task_num, task_executor_t executor);

#if BMCL_HAVE_MALLOC

sched_t* sched_create(const task_t* tasks, size_t task_num, task_executor_t executor);

void sched_destroy(sched_t* self);

#endif

bool sched_exec_next(sched_t* self, void* user_data);

void sched_exec_while_possible(sched_t* self, void* user_data);

#ifdef __cplusplus
}
#endif

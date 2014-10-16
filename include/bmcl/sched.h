#pragma once

#include <stdbool.h>
#include <stddef.h>

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

bool sched_exec_next(sched_t* self, void* user_data);

void sched_exec_while_possible(sched_t* self, void* user_data);

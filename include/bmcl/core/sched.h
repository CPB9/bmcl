/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

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
} bmcl_sched_t;

void bmcl_sched_init(bmcl_sched_t* self, const task_t* tasks, task_data_t* task_data, size_t task_num,
                     task_executor_t executor);

#if BMCL_HAVE_MALLOC

bmcl_sched_t* bmcl_sched_create(const task_t* tasks, size_t task_num, task_executor_t executor);

void bmcl_sched_destroy(bmcl_sched_t* self);

#endif

bool bmcl_sched_exec_next(bmcl_sched_t* self, void* user_data);

void bmcl_sched_exec_while_possible(bmcl_sched_t* self, void* user_data);

#ifdef __cplusplus
}
#endif

#include "../Tasks/TaskPool.h"
#include <stdio.h>

#ifndef SCHEDULE_SRTN_H
#define SCHEDULE_SRTN_H

/**
 * @brief   Schedules the Tasks in the TaskPool via the SRTN-Method
 * @param   task_pool   A pointer to the TaskPool whose Tasks are to be scheduled.
 */
void schedule_SRTN(const TaskPool *task_pool);

#endif

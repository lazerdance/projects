#include "../Tasks/TaskPool.h"
#include <stdio.h>

#ifndef SCHEDULE_MLF_H
#define SCHEDULE_MLF_H

/**
 * @brief   Schedules the Tasks in the TaskPool via the MLF-Method
 * @param   task_pool   A pointer to the TaskPool whose Tasks are to be scheduled.
 * @param   num_queues  A number determining the maximum number of queues created.
 */
void schedule_MLF(const TaskPool *task_pool, uint16_t num_queues);

#endif

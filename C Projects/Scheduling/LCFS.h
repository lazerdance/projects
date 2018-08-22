#include "../Tasks/TaskPool.h"
#include <stdio.h>

#ifndef SCHEDULE_LCFS_H
#define SCHEDULE_LCFS_H

/**
 * @brief   Schedules the Tasks in the TaskPool via the LCFS-Method
 * @param   task_pool   A pointer to the TaskPool whose Tasks are to be scheduled.
 */
void schedule_LCFS(const TaskPool *task_pool);

#endif

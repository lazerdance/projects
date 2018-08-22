#include "../Tasks/TaskPool.h"
#include <stdio.h>

#ifndef SCHEDULE_HRRN_H
#define SCHEDULE_HRRN_H

/**
 * @brief   Schedules the Tasks in the TaskPool via the HRRN-Method
 * @param   task_pool   A pointer to the TaskPool whose Tasks are to be scheduled.
 */
void schedule_HRRN(const TaskPool *task_pool);

#endif

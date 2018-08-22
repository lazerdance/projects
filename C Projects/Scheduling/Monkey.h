#include "../Tasks/TaskPool.h"
#include <stdio.h>

#ifndef SCHEDULE_MONKEY_H
#define SCHEDULE_MONKEY_H

/**
 * @brief   Schedules the Tasks in the TaskPool via the Monkey-Method. It's basically a random scheduler without any
 *          Rules. See the comments in the sourcecode to get an idea how you should implement the other schedulers
 * @param   task_pool   A pointer to the TaskPool whose Tasks are to be scheduled.
 */
void schedule_Monkey(const TaskPool *task_pool);

#endif

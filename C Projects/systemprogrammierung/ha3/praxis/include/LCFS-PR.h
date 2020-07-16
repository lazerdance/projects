/*
 * LCFS-PR scheduler.
 *
 * Schedules new arriving tasks for execution in the next tick.
 * This scheduler works preemptively.
 *
 * If the arrive of new tasks happens to be at the same time
 * a running task finishes, it hast to be guaranteed that the arrive
 * function is called first.
 */
#ifndef SOLUTION_LCFS_PR_H
#define SOLUTION_LCFS_PR_H

#include "task.h"

/*
 * Initialize data structures if needed.
 *
 * @return: 0 if success, != 0 if failed.
 */
int init_LCFS_PR();

/*
 * Frees all allocated memory used.
 *
 */
void free_LCFS_PR();

/*
 * Is called when a new task arrives that has to be scheduled.
 *
 * id    : the id of the task
 * length: the time the task runs
 */
void arrive_LCFS_PR(int id, int length);

/*
 * Is called when the next tick is reached.
 *
 * @return: def_task struct pointer from the process that
 *          was executed in that tick, NULL otherwise.
 */
def_task *tick_LCFS_PR();

/*
 * Is called when the currently executed task in the CPU is ready to retire.
 *
 */
void finish_task_LCFS_PR();

#endif //SOLUTION_LCFS_PR_H

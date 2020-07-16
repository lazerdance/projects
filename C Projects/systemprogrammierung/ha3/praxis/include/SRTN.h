#ifndef SOLUTION_SRTN_H
#define SOLUTION_SRTN_H

/*
 * SRTN scheduler.
 *
 * Schedules task in the order of their remaining computing time.
 *
 * This scheduler works preemptively.
 *
 * If the arrive of new tasks happens to be at the same time
 * a running task finishes, it has to be guaranteed that the arrive
 * function is called first.
 */

#include "task.h"

/*
 * Initialize data structures if needed.
 *
 * @return: 0 on success, != 0 otherwise.
 */
int init_SRTN();

/*
 * Frees all allocated memory used.
 */
void free_SRTN();

/*
 * Is called when a new task arrives that has to be scheduled.
 *
 * id    : the id of the task
 * length: the time the task runs
 */
void arrive_SRTN(int id, int length);

/*
 * Is called when the next tick is reached.
 *
 * @return: def_task struct pointer from the process
 *          that was executed in that tick, NULL otherwise.
 */
def_task *tick_SRTN();

/*
 * Is called when the currently executed task in the CPU is ready to retire.
 *
 */
void finish_task_SRTN();

#endif //SOLUTION_SRTN_H

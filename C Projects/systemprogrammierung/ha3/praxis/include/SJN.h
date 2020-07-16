/*
 * Shortest Job Next scheduler.
 *
 * In this scheduler, the task with the shortest execution time (length)
 * currently available is scheduled when another task finishes.
 *
 * Tasks with the same execution time are ordered according to their arrival.
 *
 * If the arrive of new tasks happens to be at the same time
 * a running task finishes, it is guaranteed that the arrive
 * function is called first.
 */

#ifndef SJN_H_
#define SJN_H_

#include "task.h"

/*
 * Initialize data structures if needed.
 *
 * @return: 0 is success, != 0 otherwise.
 */
int init_SJN();

/*
 * Frees all allocated memory used.
 */
void free_SJN();

/*
 * Is called when a new task arrives that has to be scheduled.
 *
 * id    : the id of the task
 * length: the time the task runs
 */
void arrive_SJN(int id, int length);

/*
 * Is called when the next tick is reached.
 *
 * @return: def_task struct pointer from the process
 *          that was executed in that tick, NULL otherwise.
 */
def_task *tick_SJN();

/*
 * Is called when the currently executed task in the CPU is ready to retire.
 *
 */
void finish_task_SJN();

#endif /* SJN_H_ */

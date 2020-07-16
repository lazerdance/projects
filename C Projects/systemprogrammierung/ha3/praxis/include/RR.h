/*
 * Round Robin scheduler.
 *
 * Schedules tasks in round robin fashion. Preempted tasks
 * are enqueued at the end.
 *
 * If the arrival of new tasks happens to be at the same time (tick)
 * a running task finishes, it is guaranteed that the arrive
 * function is called before the actual tick.
 *
 * In addition, if a task is completed, it is guaranteed that the finish
 * function is called before the next tick.
 */

#ifndef RR_H_
#define RR_H_

#include "task.h"

/*
 * Initialize data structures if needed.
 *
 * @param time_step: the number of ticks a task can run before it gets preempted, if another task is ready.
 *
 * @return: 0 on success, != 0 otherwise.
 */
int init_RR(int time_step);

/*
 * Frees all allocated memory used.
 */
void free_RR();

/*
 * Is called when a new task arrives that has to be scheduled.
 *
 * id    : the id of the task
 * length: the time the task runs
 */
void arrive_RR(int id, int length);

/*
 * Is called when the next tick is reached.
 *
 * @return: def_task struct pointer from the process
 *          that was executed in that tick, NULL otherwise
 */
def_task *tick_RR();

/*
 * Is called when the currently executed task in the CPU is ready to retire.
 *
 */
void finish_task_RR();

#endif /* RR_H_ */

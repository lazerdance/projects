/*
 * Multilevel-Feedback Scheduler.
 *
 * Depending on the time_step defined for the first
 * queue and the number of queues specified by num_queues
 * the intermediate queues 2 ... n - 1 have always a time_step 
 * twice as long as the previous queue. The last queue is FIFO.
 *
 * For simplicity, arriving tasks are always enqueued in the first queue
 * (with the smallest time_step). Preempted tasks (if not through another task)
 * are always enqueued in the next queue larger time_step (and finally
 * in the FIFO queue).
 *
 * If the arrival of new tasks happens to be at the same time (tick)
 * a running task finishes, it has to be guaranteed that the arrive
 * function is called before the actual tick.
 *
 */
#ifndef MLF_H_
#define MLF_H_

#include "task.h"
/*
 * Initialize data structures if needed.
 *
 * time_step: the number of ticks a task can run before
 *              it gets preempted, if another task is ready.
 * num_queues: number of queues
 *
 * @return: 0 on success, != 0 otherwise
 *
 */
int init_MLF(int time_step, int num_queues);

/*
 * Frees all allocated memory used.
 */
void free_MLF();

/*
 * Is called when a new task arrives that has to be scheduled.
 *
 * id    : the id of the task
 * length: the time the task runs
 */
void arrive_MLF(int id, int length);

/*
 * Is called when the next tick is reached.
 *
 * @return: def_task struct pointer from the
 *          process that was executed in that tick, NULL otherwise.
 */
def_task *tick_MLF();

/*
 * Is called when the currently executed task in the CPU is ready to retire.
 *
 */
void finish_task_MLF();

#endif /* MLF_H_ */

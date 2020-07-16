#ifndef TASK_H_
#define TASK_H_

/*
 * This container is used to represent a task
 * in different scheduling implementations.
 */
typedef struct task_s
{
    int id;
    int length;
} def_task;

/*
 * Causes the system to switch to the task with the supplied task struct.
 * If NULL is passed, the scheduler switches to IDLE-Mode
 *
 * @param task: the struct of new task
 *
 *  @return: old task for possible further scheduling consideration
 */
def_task *switch_task(def_task *task);

/*
 * The currently running task.
 * Equals NULL if no task is running.
 */
def_task *running_task;


#endif /* TASK_H_ */

#ifndef SCHEDULING_H
#define SCHEDULING_H

#include "../Tasks/TaskPool.h"
#include "./Monkey.h"
#include "./MLF.h"
#include "./LCFS.h"
#include "./LCFS-PR.h"
#include "./HRRN.h"
#include "./SRTN.h"

enum SCHEDULING_METHOD {
    LCFS = 0,
    LCFS_PR,
    HRRN,
    SRTN,
    MLF,
    MONKEY
};

/**
 * @brief   Applies a scheduling method to a given task pool.
 * @param   task_pool   A Pointer to the task_pool, whose objects are to be scheduled.
 * @param   method      An Enum determining the scheduling method. Either FCFS, SJN, RR, MLF, MONKEY
 * @param   quantum     Only of interest if Round Robin scheduling is selected.
 **/
void schedule(TaskPool *task_pool, enum SCHEDULING_METHOD method, uint16_t scheduling_args);

#endif

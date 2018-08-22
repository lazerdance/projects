#include "Scheduling.h"

void schedule(TaskPool *task_pool, enum SCHEDULING_METHOD method, uint16_t args) {
    switch (method) {
        case MLF:
            schedule_MLF(task_pool, args);
            break;
        case MONKEY:
            schedule_Monkey(task_pool);
            break;
         case LCFS:
            schedule_LCFS(task_pool);
            break;
        case LCFS_PR:
            schedule_LCFS_PR(task_pool);
            break;
        case HRRN:
            schedule_HRRN(task_pool);
            break;
        case SRTN:
            schedule_SRTN(task_pool);
            break;
    }
}

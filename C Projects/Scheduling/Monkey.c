#include "Monkey.h"
#include <stdlib.h>

void schedule_Monkey(const TaskPool *task_pool) {
    srand(time(NULL));
    Task *CPU = NULL;

    while (!allDone(task_pool)) {
        //NOTE: This is where you COULD check for new arrivals at the current tick...

        //NOTE: This is where you COULD select a Task for execution, based on the
        //      specifications of the algorithm (in this case randomness)...
        CPU = task_pool->task[rand() % task_pool->size];

        //NOTE: This is where you execute the Task for however long you need...
        //      (Could be one tick, could be several... it really depends on YOUR OWN ideas).
        if (execTask(CPU, 1) < 0) {
            printf("%sERROR:%s No Task selected to be executed.\n", COLOR_RED, COLOR_RESET);
            break;
        }

        //NOTE: This is where you COULD increase a variable, holding the current tick...
    }
    printf("\n");

    return;
}

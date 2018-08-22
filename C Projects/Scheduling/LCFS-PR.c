#include "LCFS-PR.h"

void schedule_LCFS_PR(const TaskPool *task_pool) 
{
    //TODO
    Task *CPU = NULL;
    int tick = 0;
    
    while (!allDone(task_pool)) {
        //NOTE: This is where you COULD check for new arrivals at the current tick...
        CPU = NULL;
        int i = 0;
        while(CPU == NULL){
            CPU = checkArrivals(task_pool, tick - i);
            if(isDone(CPU))
                CPU = NULL;
            i++;
        }

        //NOTE: This is where you execute the Task for however long you need...
        //      (Could be one tick, could be several... it really depends on YOUR OWN ideas).
        if (execTask(CPU, 1) < 0) {
            printf("%sERROR:%s No Task selected to be executed.\n", COLOR_RED, COLOR_RESET);
            break;
        }
        

        tick++;

        //NOTE: This is where you COULD increase a variable, holding the current tick...
    }
    printf("\n");
}

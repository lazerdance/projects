#include "HRRN.h"

void schedule_HRRN(const TaskPool *task_pool) 
{
    //TODO
    Task* CPU = NULL;
    int i, tick = 0;
    double CPU_RR, tmp_RR;
    
    while (!allDone(task_pool)) {
        //NOTE: This is where you COULD check for new arrivals at the current tick...
        if(CPU == NULL || isDone(CPU)){
            i = 0;
            Task *tmp = NULL;
            CPU = NULL;
            while(i <= tick){
                if(CPU == NULL){
                    CPU = checkArrivals(task_pool, tick - i);
                        if(isDone(CPU))
                            CPU = NULL;
                }else{
                    tmp = checkArrivals(task_pool, tick - i);
                    tmp_RR, CPU_RR = 0;
                    if(tmp != NULL){
                        //tick - arrival_tick = wartezeit
                        tmp_RR = (double) (tmp->total_ticks + (tick - tmp->arrival_tick)) / tmp->total_ticks;
                        CPU_RR = (double) (CPU->total_ticks + (tick - CPU->arrival_tick)) / CPU->total_ticks;
                        //printf("tmp_RR: %f - CPU_RR: %f", tmp_RR, CPU_RR);
                    }
                    if(tmp != NULL && !isDone(tmp) && tmp_RR > CPU_RR)
                        CPU = tmp;
                }
                i++;
            }
        }

        //NOTE: This is where you execute the Task for however long you need...
        //      (Could be one tick, could be several... it really depends on YOUR OWN ideas).
        if (execTask(CPU, 1) < 0) {
            printf("%sERROR:%s No Task selected to be executed.\n", COLOR_RED, COLOR_RESET);
            break;
        }
        

        tick++;
    }
    printf("\n");
}

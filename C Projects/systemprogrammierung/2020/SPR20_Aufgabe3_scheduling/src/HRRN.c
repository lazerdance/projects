#include "../lib/HRRN.h"
#include <stdio.h>

int time = 0;

process* HRRN_tick (process* running_process){
    if (running_process==NULL || running_process->time_left==0){
        running_process=queue_poll_hrrn(HRRN_queue,time);
    }
    if (running_process!=NULL){
        running_process->time_left--;
    
    }
    time++;
    return running_process;

}

int HRRN_startup(){
    HRRN_queue=new_queue();
    if (HRRN_queue==NULL){
        return 1;
    }
    return 0;
}

process* HRRN_new_arrival(process* arriving_process, process* running_process){
    if(arriving_process!=NULL){
        queue_add(arriving_process, HRRN_queue);
    }
    return running_process;
}

void HRRN_finish(){
    free_queue(HRRN_queue);
}
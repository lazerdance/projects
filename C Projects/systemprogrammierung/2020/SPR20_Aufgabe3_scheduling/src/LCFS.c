#include "../lib/LCFS.h"


int LCFS_startup(){
    LCFS_queue=new_queue();
    if (LCFS_queue==NULL){
        return 1;
    }
    return 0;
}

process* LCFS_tick (process* running_process){
    if (running_process==NULL || running_process->time_left==0){
        running_process=queue_pop(LCFS_queue);
    }
    if (running_process!=NULL){
        running_process->time_left--;
    
    }

    return running_process;
}


process* LCFS_new_arrival(process* arriving_process, process* running_process){
    if(arriving_process!=NULL){
        queue_add(arriving_process, LCFS_queue);
    }
    return running_process;
}

void LCFS_finish(){
    free_queue(LCFS_queue);
}
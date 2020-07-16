#include "../lib/PRIOP.h"
#include "../lib/queue.h"
#include <stdio.h>


process* PRIOP_tick (process* running_process){
    if (running_process==NULL || running_process->time_left==0){
        running_process=queue_poll_priop(PRIOP_queue);
    }
    if (running_process!=NULL){
        running_process->time_left--;
    
    }

    return running_process;
}

int PRIOP_startup(){
    PRIOP_queue=new_queue();
    if (PRIOP_queue==NULL){
        return 1;
    }
    return 0;
}

process* PRIOP_new_arrival(process* arriving_process, process* running_process){
    if(arriving_process!=NULL){
        queue_add(arriving_process, PRIOP_queue);
    }
    if(running_process!=NULL)
        queue_add(running_process,PRIOP_queue);
    return queue_poll_priop(PRIOP_queue);
}

void PRIOP_finish(){
    free_queue(PRIOP_queue);
}
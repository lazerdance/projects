#include "../lib/SRTNP.h"

process* SRTNP_tick (process* running_process){
    if (running_process==NULL || running_process->time_left==0){
        running_process=queue_poll_srtnp(SRTNP_queue);
    }
    if (running_process!=NULL){
        running_process->time_left--;
    
    }

    return running_process;

}

int SRTNP_startup(){
    SRTNP_queue=new_queue();
    if (SRTNP_queue==NULL){
        return 1;
    }
    return 0;
}

process* SRTNP_new_arrival(process* arriving_process, process* running_process){
    if(arriving_process!=NULL){
        queue_add(arriving_process, SRTNP_queue);
    }
    if(running_process!=NULL)
        queue_add(running_process,SRTNP_queue);
    return queue_poll_srtnp(SRTNP_queue);
}

void SRTNP_finish(){
    free_queue(SRTNP_queue);
}
#include "../lib/RR.h"
#include <stdio.h>

int counter = 0;
int RR_quantum = 999;

process* RR_tick (process* running_process){
    if(running_process == NULL)
        running_process = queue_poll(RR_queue);
    else if(counter == RR_quantum && running_process->time_left != 0){
        counter = 0;
        queue_add(running_process, RR_queue);
        running_process = queue_poll(RR_queue);
    }
    else if(running_process->time_left==0){    
        counter = 0;
        running_process = queue_poll(RR_queue);
    }
    
    if (running_process!=NULL){
        running_process->time_left--;
        counter++;
    }

    return running_process;
}

int RR_startup(int quantum){
    RR_quantum = quantum;
    RR_queue=new_queue();
    if (RR_queue==NULL){
        return 1;
    }
    return 0;
}


process* RR_new_arrival(process* arriving_process, process* running_process){
    if(arriving_process!=NULL){
        queue_add(arriving_process, RR_queue);
    }
    return running_process;
}


void RR_finish(){
    free_queue(RR_queue);
}
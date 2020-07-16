#include "../lib/MLF.h"

int ticks = 0;
int next_queue;

int MLF_startup(){
    MLF_queue=new_queue();
    MLF_queue2=new_queue();
    MLF_queue3=new_queue();
    MLF_queue4=new_queue();
    if (MLF_queue==NULL||MLF_queue2==NULL||MLF_queue3==NULL||MLF_queue4==NULL){
        return 1;
    }
    return 0;
}

void add_to_queue(process* running_process){
    switch(next_queue){
        case 2:
            queue_add(running_process,MLF_queue2);
            break;
        case 3:
            queue_add(running_process,MLF_queue3);
            break;
        case 4:
            queue_add(running_process,MLF_queue4);
            break;
    }
}

/*
 * ticks processes according to MLF order mentioned in the lecture slides
 * depending on which level the process is ticks and next queue will define its parameters for next cycle
 */
process* MLF_tick (process* running_process){
    if (running_process==NULL || running_process->time_left==0 || ticks == 0){
        if(running_process != NULL && running_process->time_left > 0){
            add_to_queue(running_process);            
        }
        running_process=queue_poll(MLF_queue);
        ticks = 1;
        next_queue = 2;
        if(running_process == NULL){
            running_process=queue_poll(MLF_queue2);
            ticks = 2;
            next_queue = 3;
        }
        if(running_process == NULL){
            running_process=queue_poll(MLF_queue3);
            ticks = 4;
            next_queue = 4;
        }
        if(running_process == NULL){
            running_process=queue_poll(MLF_queue4);
            ticks = -1;                 // keep ticking until process is done
            next_queue = -1;
        }
    }
    if (running_process!=NULL){
        running_process->time_left--;
        ticks--;
    }

    return running_process;
}

process* MLF_new_arrival(process* arriving_process, process* running_process){
    if(arriving_process!=NULL){
        queue_add(arriving_process, MLF_queue);
    }
    return running_process;
}


void MLF_finish(){
    free_queue(MLF_queue);
    free_queue(MLF_queue2);
    free_queue(MLF_queue3);
    free_queue(MLF_queue4);
}
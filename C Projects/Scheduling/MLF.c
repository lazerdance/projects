#include "MLF.h"
#include "queue.h"
#include <math.h>

//Hilfsfunktion zum log2 berechnen
int log_2(int n){
   // printf("%d\n",n);
    //printf("%f",(log(n)/log(2)));
    return (int) (log(n)/log(2));
}
void schedule_MLF(const TaskPool *task_pool, uint16_t num_levels) {
    //TODO
    Task *CPU = NULL;
    int tick = 0;
    int scheibe = 0;
    int scheibe_max = 0;
    queue* level_queue[num_levels];
    for(int i = 0; i < num_levels; ++i){
        level_queue[i] = create_queue();
    }
    while (!allDone(task_pool)) {
        //NOTE: This is where you COULD check for new arrivals at the current tick...
        if(checkArrivals(task_pool,tick)!= NULL){
            add_elem(level_queue[0], checkArrivals(task_pool,tick));    //neuer prozess in lvl 0 queue einfuegen
            
        }
        if(scheibe == scheibe_max && CPU != NULL){
            //printf("execticks : %d\n",CPU->exec_ticks);
            //printf("%d", log_2((int) CPU->exec_ticks + 1));
            if(!isDone(CPU)){
                add_elem(level_queue[log_2((int) CPU->exec_ticks + 1)], CPU);//prozess in ein level hoeher einfuegen
            }
            CPU = NULL;
            scheibe = 0;
        }
        
        int lvl_count = 0;
        while(CPU == NULL){
            ////printf("Poll!!");
            CPU = poll(level_queue[lvl_count]);
            scheibe_max = pow(2, lvl_count);
            ++lvl_count;
        }
        if (execTask(CPU, 1) < 0) {
            printf("%sERROR:%s No Task selected to be executed.\n", COLOR_RED, COLOR_RESET);
            break;
        }
        
        
        ++scheibe;
        ++tick;

        //NOTE: This is where you COULD increase a variable, holding the current tick...
    }
    printf("\n");

    return;
    printf("NOT IMPLEMENTED!\n");
}

#include"./../include/worker.h"
#include"./../include/order.h"
#include"./../include/management.h"
#include "./../include/warehouse.h"
#include <stdlib.h>
#include <pthread.h>

#include<stdio.h>


struct worker{    
    unsigned int muffin_made;
   
    warehouse* wh;
    order* o;
    management* m;    
};

//FUNCTION CALLED BY BAKERY
worker* worker_create(warehouse* wh, management* m){
    worker* new_worker = (worker*) malloc(sizeof(worker));
    if(new_worker == NULL){
        return NULL;
    }else{
        new_worker->wh = wh;
        new_worker->m = m;
        new_worker->muffin_made = 0;
        
        return new_worker;
    }
}



//THREAD FUNCTION
void* baking(void* wor){
    worker* w = (worker*)wor;
    w->o = get_order_from_management(w->m);
    
    if(get_client(w->o) == NULL)
        pthread_exit((void*)1);

    while(1){
        
        if(w->o == NULL){
            pthread_exit(NULL);
        }
        make_muffin(w);
        if(get_muffin(w->o) == w->muffin_made){
            deposit_finished_orders(w->m,w->o);
            w->muffin_made = 0;
            w->o = get_order_from_management(w->m);
        }
    }
}


void make_muffin(worker* w){
   
    get_flour(FLOUR_FOR_ONE_MUFFIN, w->wh);    
    get_sugar(SUGAR_FOR_ONE_MUFFIN, w->wh);
    get_choclate(CHOCLATE_FOR_ONE_MUFFIN, w->wh);
    w->muffin_made++;
}

//FUNCTION CALLED BY BAKERY
void worker_destroy(worker* w){

	free(w);
}

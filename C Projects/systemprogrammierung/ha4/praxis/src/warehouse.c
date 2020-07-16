#include "./../include/warehouse.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

struct warehouse{
    unsigned int flour, sugar, choclate;
    int locked;
    //bzuh
    
    unsigned int sugar_taken, flour_taken, choclate_taken;
    //TODO
    pthread_mutex_t * whsugar_m;
    pthread_mutex_t * whflour_m;
    pthread_mutex_t * whchoclate_m;
	pthread_cond_t * enoughC_c;
	pthread_cond_t * enoughF_c;
	pthread_cond_t * enoughS_c;
};

warehouse* warehouse_create(){
    warehouse* new_warehouse = (warehouse*) malloc(sizeof(warehouse));
    if(new_warehouse== NULL){
        printf("Unable to allocate memory in function %s()\n",__func__);
        return NULL;
    }
    new_warehouse->flour = 0;
    new_warehouse->sugar = 0;
    new_warehouse->choclate = 0;
    new_warehouse->locked = 0;
    
    //TODO
    new_warehouse->whsugar_m = malloc(sizeof(pthread_mutex_t));
    new_warehouse->whflour_m = malloc(sizeof(pthread_mutex_t));
    new_warehouse->whchoclate_m = malloc(sizeof(pthread_mutex_t));    
    pthread_mutex_init(new_warehouse->whsugar_m, NULL);
    pthread_mutex_init(new_warehouse->whflour_m, NULL);
    pthread_mutex_init(new_warehouse->whchoclate_m, NULL);

	new_warehouse->enoughC_c = malloc(sizeof(pthread_cond_t));
    new_warehouse->enoughF_c = malloc(sizeof(pthread_cond_t));
    new_warehouse->enoughS_c = malloc(sizeof(pthread_cond_t));    
    pthread_cond_init(new_warehouse->enoughC_c, NULL);
    pthread_cond_init(new_warehouse->enoughF_c, NULL);
    pthread_cond_init(new_warehouse->enoughS_c, NULL);

    
    new_warehouse->choclate_taken = 0;
    new_warehouse->flour_taken = 0;
    new_warehouse->sugar_taken = 0;
    return new_warehouse;
}

//FUNCTIONS CALLED BY WORKER THREADS
//@param: amount of flour/sugar/choclate to be taken from warehouse wh

unsigned int get_choclate(unsigned int amount, warehouse* wh){
	//TODO
	pthread_mutex_lock(wh->whchoclate_m);
	while(wh->choclate >= amount) {
		pthread_cond_wait(wh->enoughC_c,wh->whchoclate_m);
	}
    wh->choclate_taken += amount;
    wh->choclate -= amount;
    pthread_mutex_unlock(wh->whchoclate_m);
    return amount;
}

unsigned int get_flour(unsigned int amount, warehouse* wh){
    //TODO
	pthread_mutex_lock(wh->whflour_m);
	while(wh->flour >= amount) {
		pthread_cond_wait(wh->enoughF_c,wh->whflour_m);
	}
    wh->flour_taken += amount;
    wh->flour -= amount;
    pthread_mutex_unlock(wh->whflour_m);
    return amount;
}

unsigned int get_sugar(unsigned int amount, warehouse* wh){
    //TODO
	pthread_mutex_lock(wh->whsugar_m);
	while(wh->sugar >= amount) {
		pthread_cond_wait(wh->enoughC_c,wh->whsugar_m);
	}
    wh->sugar_taken += amount;
    wh->sugar -= amount;
    pthread_mutex_unlock(wh->whsugar_m);   
    return amount;
}

//own function----->----->----->----->----->----->----->----->----->----->----->
//supplier needs signals from warehouse
void *get_signalWh(warehouse *wh, char ingredient){
    if (ingredient == 'c'){
        return wh->whchoclate_m;
    }
    else if (ingredient == 'f'){
        return wh->whflour_m;
    }
    else if (ingredient == 's'){
        return wh->whsugar_m;
    }
    return NULL;
}

//FUNCTIONS CALLED BY SUPPLIER THREADS
//@param: amount of flour/sugar/choclate to be store in warehouse wh
void deposit_choclate(unsigned int amount, warehouse* wh){
    //TODO
    pthread_mutex_lock(wh->whchoclate_m);
    if(wh->choclate + amount > MAX_STORE_CAPACITY){
        wh->choclate = MAX_STORE_CAPACITY;
    }else{
        wh->choclate += amount;
    }
    pthread_mutex_unlock(wh->whchoclate_m);
    return;
}

void deposit_sugar(unsigned int amount, warehouse* wh){
    //TODO
    pthread_mutex_lock(wh->whsugar_m);
    if(wh->sugar + amount > MAX_STORE_CAPACITY){
        wh->sugar = MAX_STORE_CAPACITY;
    }else{
        wh->sugar += amount;
    }
    pthread_mutex_unlock(wh->whsugar_m);
    return;
}

void deposit_flour(unsigned int amount, warehouse* wh){
    //TODO
    pthread_mutex_lock(wh->whflour_m);
    if(wh->flour + amount > MAX_STORE_CAPACITY){
        wh->flour = MAX_STORE_CAPACITY;
    }else{
        wh->flour += amount;
    }
    pthread_mutex_unlock(wh->whflour_m);
    return;
}


//FUNCTIONS CALLED BY BAKERY
void lock_warehouse(warehouse* wh){
    //TODO
    wh->locked=1;

}

unsigned int get_used_resources(warehouse* wh, int type){
    if(type == SUGAR) return wh->sugar_taken;
    if(type == FLOUR) return wh->flour_taken;
    if(type == CHOCLATE) return wh->choclate_taken;
    return 0;
}


void warehouse_destroy(warehouse* wh){
    //TODO
    pthread_mutex_destroy(wh->whchoclate_m);
    pthread_mutex_destroy(wh->whsugar_m);
    pthread_mutex_destroy(wh->whflour_m);
    free(wh->whchoclate_m);
    free(wh->whsugar_m);
    free(wh->whflour_m);

	pthread_cond_destroy(wh->enoughC_c);
    pthread_cond_destroy(wh->enoughS_c);
    pthread_cond_destroy(wh->enoughF_c);
    free(wh->enoughC_c);
    free(wh->enoughS_c);
    free(wh->enoughF_c);
	
    free(wh);
    return;
}

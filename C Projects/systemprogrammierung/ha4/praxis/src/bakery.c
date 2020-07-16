#include"./../include/order.h"
#include"./../include/supplier.h"
#include"./../include/management.h"
#include"./../include/worker.h"
#include"./../include/warehouse.h"


#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#define num_worker_threads 5
char* filename = "orders.txt";
int return_value = 0;

int main(){
    
        //create all the instances
        warehouse* wh = warehouse_create();
        if(wh == NULL){
            printf("Warehouse konnte nicht erstellt werden\n");
            return -1;
        }
        
        
      forwarding_agent* fa = forwarding_agent_create(wh);
        if(fa == NULL){
            printf("forwarding_agent konnte nicht erstellt werden\n");
            return -2;
        }
        
        management* m = management_create(filename, num_worker_threads, fa);
        if(m == NULL){
            printf("management konnte nicht erstellt werden\n");
            return -3;
        }
        
        
        worker* workers[num_worker_threads];
        for(int i = 0; i<num_worker_threads;++i){
            workers[i] = worker_create(wh, m);
            if(workers[i] == NULL){
                return -4;
            }
        }
        
        //create all the threads
        pthread_t forwarding_agent_thread;        
        pthread_t management_thread;        
        pthread_t worker_threads[num_worker_threads];
        int worker_return_value[num_worker_threads];
        
        
        pthread_create(&forwarding_agent_thread,NULL,forwarding_agent_working,fa);      
        pthread_create(&management_thread,NULL,get_and_deposit_new_orders, m);
        
        for(int i = 0; i<num_worker_threads;++i){
            pthread_create(&worker_threads[i],NULL,baking,workers[i]);
        }
        
        //wait for threads
        pthread_join(management_thread,NULL);
        for(int i = 0; i< num_worker_threads; ++i){
            pthread_join(worker_threads[i],(void**)&worker_return_value[i]);
        }
        for(int i = 0; i< num_worker_threads;++i){
            if(worker_return_value[i]==1){
                printf("Not all worker threads are working correctly\n");
                return_value = 1;
            }
        }
        
        //CLEAN UP
        
        //locking the warehouse is the signal to the suppliers to end their job
          lock_warehouse(wh);
        
        //destroy workers
        for(int i = 0; i< num_worker_threads;++i){
            worker_destroy(workers[i]);
            
        }
        
        
        management_destroy(m);
        
        pthread_join(forwarding_agent_thread,NULL);
        printf("sugar %u g and FLOUR %u g and choclate %u g was taken \n", get_used_resources(wh, SUGAR),get_used_resources(wh, FLOUR),get_used_resources(wh,CHOCLATE));
        warehouse_destroy(wh);
        forwarding_agent_destroy(fa);
        return return_value;
}

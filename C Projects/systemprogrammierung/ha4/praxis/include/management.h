#ifndef MANAGEMENT_H
#define MANAGEMENT_H


#include"order.h"
#include"supplier.h"

typedef struct management management;

management* management_create(char* filename, unsigned int num_worker_threads, forwarding_agent* fa);

void* get_and_deposit_new_orders(void* m);
void deposit_finished_orders(management*m, order* o);
order* get_order_from_management(management* m);
unsigned int management_destroy(management* m);

order_list* get_orders_in(management* m);

#endif

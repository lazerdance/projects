#ifndef SUPPLIER_H
#define SUPPLIER_H

#include"warehouse.h"


typedef struct forwarding_agent forwarding_agent;
forwarding_agent* forwarding_agent_create(warehouse* wh);
void* forwarding_agent_working(void* fa);
void stop_forwarding_agent(forwarding_agent* fa);
void forwarding_agent_destroy(forwarding_agent* fa);
void *get_signal(forwarding_agent* fa);

typedef struct supplier supplier;
supplier* supplier_create(warehouse* wh);
void* deliver_choclate(void* s);
void* deliver_sugar(void* s);
void* deliver_flour(void* s);
void end_job(supplier* s);
void supplier_destroy(supplier* s);



#endif

#ifndef WORKER_H
#define WORKER_H

#define CHOCLATE_FOR_ONE_MUFFIN 50
#define SUGAR_FOR_ONE_MUFFIN 100
#define FLOUR_FOR_ONE_MUFFIN 200

#include "warehouse.h"
#include "management.h" 
typedef struct worker worker;

worker* worker_create(warehouse* wh, management* m);
void make_muffin(worker* w);
void* baking(void* w);
void worker_destroy(worker* w);
#endif

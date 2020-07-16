#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#define MAX_STORE_CAPACITY 500
#define CHOCLATE 0
#define SUGAR 1
#define FLOUR 2

typedef struct warehouse warehouse;

warehouse* warehouse_create();

unsigned int get_choclate(unsigned int amount, warehouse* wh);
unsigned int get_flour(unsigned int amount, warehouse* wh);
unsigned int get_sugar(unsigned int amount, warehouse* wh);

void deposit_choclate(unsigned int amount, warehouse* wh);
void deposit_sugar(unsigned int amount, warehouse* wh);
void deposit_flour(unsigned int amount, warehouse* wh);
void *get_signalWh(warehouse* wh, char ingredient);


unsigned int get_used_resources(warehouse*, int type);

void lock_warehouse(warehouse* wh);

void warehouse_destroy(warehouse* wh);

#endif

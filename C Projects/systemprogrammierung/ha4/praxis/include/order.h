#ifndef ORDER_H
#define ORDER_H

#define MAX_ORDERS 20

typedef struct order order;

order* order_create(char* client, unsigned int muffin);
char* get_client(order* o);
unsigned int get_muffin(order* o);
void order_destroy(order* o);

typedef struct order_list order_list;

order_list* order_list_create();
unsigned int get_count(order_list* ol);
void deposit_order(order_list* ol, order* o);
order* get_order(order_list* ol);
void order_list_destroy(order_list* ol);


#endif

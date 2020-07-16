/*
    Rechnernetze und Verteilte Systeme - Block 3; Aufgabe 6;
    Gruppe: T18G01
*/

#ifndef protokoll_h
#define protokoll_h

typedef struct _package_header {
    uint8_t get;
    uint8_t set;
    uint8_t del;
    uint8_t ack;
    uint8_t transid;
    unsigned int key_size;
    unsigned int value_size;
} package_header;

package_header unmarshall(void *buffer);
unsigned int marshall_query(void **result, uint8_t operation, const char *key, const char *value);
unsigned int marshall_answer(void **result, void *buffer, void* value, unsigned int value_size, package_header header);

void protokoll_debug(void *buffer, package_header head);

#endif

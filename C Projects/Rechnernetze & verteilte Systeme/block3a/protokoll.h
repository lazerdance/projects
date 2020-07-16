/*
    Rechnernetze und Verteilte Systeme - Block 3; Aufgabe 5;
    Gruppe: T18G01
*/

#ifndef protokoll_h
#define protokoll_h

typedef struct package_header_struct {
    uint8_t get;
    uint8_t set;
    uint8_t del;
    uint8_t transid;
    unsigned int key_size;
    unsigned int value_size;
} package_header;

package_header unmarshall(void *buffer);

void protokoll_debug(void *buffer, package_header head);
unsigned int marshall(void **result, void *buffer, void* value, unsigned int value_size, package_header header);

#endif

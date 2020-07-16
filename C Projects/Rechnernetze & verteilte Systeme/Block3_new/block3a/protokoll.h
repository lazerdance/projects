/*
    Rechnernetze und Verteilte Systeme - Block 3; Aufgabe 6;
    Gruppe: T18G01
*/

#ifndef protokoll_h
#define protokoll_h

typedef struct _package_header {
    uint8_t rcp_cmd;
    unsigned int key_length;
    unsigned int value_length;
} package_header;

package_header unmarshall(void *buffer);
unsigned int marshall_query(void **result, uint8_t operation, const char *key, const char *value);
unsigned int marshall_answer(void **result, void *buffer, void* value, unsigned int value_length, package_header header);

void print_report(void *buffer, package_header header);
#endif

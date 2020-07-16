/*
    Rechnernetze und Verteilte Systeme - Block 4; Aufgabe 3;
    Gruppe: T18G01
*/

#ifndef protokoll_h
#define protokoll_h

typedef struct _package_header {
	uint8_t intern;
    uint8_t get;
    uint8_t set;
    uint8_t del;
    uint8_t ack;
    unsigned int key_size;
    unsigned int value_size;
    
    uint8_t lookup;
    uint8_t reply;
    unsigned short hashID;
    unsigned short id;
    uint32_t ip;
    unsigned short port;
} package_header;

unsigned int marshall_answer(void **result, void *buffer, void* value, unsigned short value_size,
    package_header header);
unsigned int marshall_intern(void **result, void *buffer, void* value, unsigned short value_size,
    package_header header);

package_header unmarshall(void *buffer);
void protokoll_debug(void *buffer, package_header head, const char* argv[]);

#endif

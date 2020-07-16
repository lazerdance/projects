/*
    Rechnernetze und Verteilte Systeme - Block 5; Aufgabe 4;
    Gruppe: T18G01
*/

#ifndef protokoll_h
#define protokoll_h

typedef struct _package_header {
	uint8_t intern;
    uint8_t join;
    uint8_t notify;
    uint8_t stabilize;
    uint8_t get;
    uint8_t set;
    uint8_t del;
    uint8_t ack;
    uint8_t transid;
    unsigned int key_size;
    unsigned int value_size;
    unsigned short id;
    unsigned short port;
    uint32_t ip;
} package_header;

unsigned int marshall_answer(void **result, void *buffer, void* value, unsigned short value_size,
    package_header header);
unsigned int marshall_intern(void **result, void *buffer, void* value, unsigned short value_size,
    package_header header);

package_header unmarshall(void *buffer);
void protokoll_debug(void *buffer, package_header header,
    int preID, int sucID, char* preIP, char* prePort, char* sucIP, char* sucPort);

#endif

/*
    Rechnernetze und Verteilte Systeme - Block 3; Aufgabe 6;
    Gruppe: T18G01
*/

#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>

#include "memory.h"
#include "protokoll.h"

unsigned int marshall_query(void **result, uint8_t operation, const char *key, const char *value) {
    unsigned int key_length = strlen(key);
    unsigned int value_length = value ? strlen(value) : 0;
    unsigned short key_length_nb = htons(key_length);
    unsigned short value_length_nb = htons(value_length);
    unsigned int package_size = 6 + key_length + value_length;
    char *package = myalloc(package_size);

    memcpy(package, &operation, 1);
    memcpy(package + 2, &key_length_nb, 2);
    memcpy(package + 4, &value_length_nb, 2);
    memcpy(package + 6, key, key_length);
    if (value) memcpy(package + 6 + key_length, value, value_length);

    *result = (void *) package;
    return package_size;
}

unsigned int marshall_answer(void **result, void *buffer, void* value, unsigned int value_size, package_header header) {
    unsigned int package_size = 6 + (header.key_size + value_size) * header.get;
    unsigned int value_size_nb;
    char *package = myalloc(package_size);

    uint8_t byte = 8 + header.get * 4 + header.set * 2 + header.del;
    memcpy(package, &byte, 1);                                                      // write first header byte
    memcpy(package + 1, (void *) ((char *) buffer + 1), 1);                         // write transactionID

    if (value_size != 0 && value) {
        value_size_nb = htons(value_size);
        memcpy(package + 4, &value_size_nb, 2);
        memcpy(package + 2, (void *) ((char *) buffer + 2), 2);
        memcpy(package + 6, (void *) ((char *) buffer + 6), header.key_size);
        memcpy(package + 6 + header.key_size, value, value_size);
    }

    *result = (void *) package;
    return package_size;
}

package_header unmarshall(void *buffer) {
    package_header header;

    uint8_t *byte = myalloc(1);
    memcpy(byte, buffer, 1);
    header.ack = (*byte >> 3) & 0x1;
    header.get = (*byte >> 2) & 0x1;
    header.set = (*byte >> 1) & 0x1;
    header.del = *byte & 0x1;

    memcpy(byte, (char *) buffer + 1, 1);
    header.transid = *byte;
    free(byte);

    unsigned int *size = myalloc(sizeof(size));
    memcpy(size, (char *) buffer + 2, 2);
    header.key_size = ntohs(*size);

    memcpy(size, (char *) buffer + 4, 2);
    header.value_size = ntohs(*size);
    free(size);

    return header;
}

void protokoll_debug(void *buffer, package_header header) {
    void *key = (void *) ((char *) buffer + 6);
    void *value = (void *) ((char *) buffer + 6 + header.key_size);

    char *key_str = myalloc(header.key_size + 1 * sizeof(char));
    memcpy(key_str, key, header.key_size);

    char *value_str = myalloc(header.value_size + 1 * sizeof(char));
    memcpy(value_str, value, header.value_size);

    // printf("\nBuffer: %x\n", *(unsigned int *) buffer);
    printf("Get %d\nSet %d\nDel %d\n", header.get, header.set, header.del);
    printf("Ack: %d\n", header.ack);
    printf("KeySize: %d\n", header.key_size);
    printf("Key: %s\n", (char *) key_str);
    printf("ValueSize: %d\n", header.value_size);
    printf("Value: %s\n", (char *) value_str);
    printf("TransID: %d\n", header.transid);

    free(key_str);
    free(value_str);
    return;
}

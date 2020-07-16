/*
    Rechnernetze und Verteilte Systeme - Block 5; Aufgabe 4;
    Gruppe: T18G01
*/

#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include "memory.h"
#include "protokoll.h"

unsigned int marshall_answer(void **result, void *buffer, void* value, unsigned short value_size,
    package_header header) {

    unsigned int package_size = 6 + (header.key_size + value_size) * header.get;
    char *package = myalloc(package_size);
    unsigned short value_nb;

    uint8_t byte = 8 + header.get * 4 + header.set * 2 + header.del;
    memcpy(package, &byte, 1);                                                      // write first header byte
    memcpy(package + 1, &header.transid, 1);                                        // write transactionID

    if (value_size != 0 && value) {
        value_nb = htons(value_size);
        memcpy(package + 4, &value_nb, 2);
        memcpy(package + 2, (void *) ((char *) buffer + 2), 2);
        memcpy(package + 6, (void *) ((char *) buffer + 6 + (8 * header.intern)), header.key_size);
        memcpy(package + 6 + header.key_size, value, value_size);
    }

    *result = (void *) package;
    return package_size;
}

unsigned int marshall_intern(void **result, void *buffer, void* value, unsigned short value_size,
    package_header header) {

    unsigned int package_size = 14 + header.key_size * (header.get || !header.ack)
        + value_size * (header.get || (!header.ack && header.set));
    char *package = myalloc(package_size);
    unsigned short value_nb;
    uint32_t myip_nb;

    uint8_t byte = 128 + 64 * header.join + 32 * header.notify + 16* header.stabilize;
    byte = byte + (8 * header.ack) + header.get * 4 + header.set * 2 + header.del;
    memcpy(package, &byte, 1);                                                      // write first header byte
    memcpy(package + 1, &header.transid, 1);                                        // write transID
    value_nb = htons(header.id);
    myip_nb = htonl(header.ip);
    memcpy(package + 6, &value_nb, 2);
    memcpy(package + 8, &myip_nb, 4);
    memcpy(package + 12, &header.port, 2);

    if (header.join || header.notify || header.stabilize) {
        *result = (void *) package;
        return package_size;
    }

    if (header.get || !header.ack) {
        memcpy(package + 2, (void *) ((char *) buffer + 2), 2);
        memcpy(package + 14, (void *) ((char *) buffer + 6 + (8 * header.intern)), header.key_size);
    }

    if (header.get || (!header.ack && header.set)) {
        value_nb = htons(value_size);
        memcpy(package + 4, &value_nb, 2);
        memcpy(package + 14 + header.key_size, value, value_size);
    }

    *result = (void *) package;
    return package_size;
}

package_header unmarshall(void *buffer) {
    package_header header;

    uint8_t *byte = myalloc(1);
    memcpy(byte, buffer, 1);
    header.intern = (*byte >> 7) & 0x1;
    header.join = (*byte >> 6) & 0x1;
    header.notify = (*byte >> 5) & 0x1;
    header.stabilize = (*byte >> 4) & 0x1;
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

    if (header.intern) {
        memcpy(size, (char *) buffer + 6, 2);
        header.id = ntohs(*size);

        uint32_t *ipaddr = myalloc(sizeof(ipaddr));
        memcpy(ipaddr, (char *) buffer + 8, 4);
        header.ip = ntohl(*ipaddr);
        free(ipaddr);

        memcpy(size, (char *) buffer + 12, 2);
        header.port = *size;
    }

    free(size);
    return header;
}

void protokoll_debug(void *buffer, package_header header,
    int preID, int sucID, char* preIP, char* prePort, char* sucIP, char* sucPort) {

    void *key, *value;
    char portstr[8];
    char ip4str[INET_ADDRSTRLEN];                       // space to hold the IPv4 string
    struct sockaddr_in sa;                              // pretend this is loaded with something

    printf("=============================\n");
    // printf("\nBuffer: %x\n", *(unsigned int *) buffer);
    printf("Intern:  %d Ack:    %d TransID:   %d\n", header.intern, header.ack, header.transid);
    printf("Get:     %d Set:    %d Del:       %d\n", header.get, header.set, header.del);
    printf("KeySize: %d           ValueSize: %d\n", header.key_size, header.value_size);

    if (header.intern) {
        printf("Join:    %d Notify: %d Stabilize: %d\n", header.join, header.notify, header.stabilize);
        sa.sin_addr.s_addr = header.ip;
        snprintf(portstr, 7, "%d", ntohs(header.port));
        inet_ntop(AF_INET, &(sa.sin_addr), ip4str, INET_ADDRSTRLEN);
        printf("\nID:    %d\tIP:    %s\tPort:    %s\n", header.id, ip4str, portstr);
        key = (void *) ((char *) buffer + 14);
        value = (void *) ((char *) buffer + 14 + header.key_size);
    } else {
        key = (void *) ((char *) buffer + 6);
        value = (void *) ((char *) buffer + 6 + header.key_size);
    }

    printf("preID: %d\tpreIP: %s\tprePort: %s\n", preID, preIP, prePort);
    printf("sucID: %d\tsucIP: %s\tsucPort: %s\n", sucID, sucIP, sucPort);

    char *key_str = myalloc(header.key_size + 1);
    memcpy(key_str, key, header.key_size);

    char *value_str = myalloc(header.value_size + 1);
    memcpy(value_str, value, header.value_size);

    if (header.key_size) printf("Key:   %s\n", (char *) key_str);
    if (header.value_size) printf("Value: %s\n", (char *) value_str);

    free(key_str);
    free(value_str);

    printf("=============================\n");

    return;
}

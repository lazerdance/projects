/*
    Rechnernetze und Verteilte Systeme - Block 4; Aufgabe 3;
    Gruppe: T18G01
*/

#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include "protokoll.h"

unsigned int marshall_answer(void **result, void *buffer, void* value, unsigned short value_size,
    package_header header) {
    // printf("marshalling size 7 + %d + %d\n",header.key_size,value_size);
    unsigned int package_size = 7 + header.key_size + value_size;
    char *package = calloc(package_size,1);
    unsigned long value_nb;
    uint16_t key_length_nb;
    uint8_t byte = 8 + header.get * 4 + header.set * 2 + header.del;
    memcpy(package, &byte, 1);                                                      // write first header byte

    if (value_size != 0 && value) {
        value_nb = htonl(value_size);
        key_length_nb = htons(header.key_size);
        memcpy(package + 1, &key_length_nb, 2);
        memcpy(package + 3, &value_nb, 4);
        printf("marshal.intern is %d\n",header.intern);
        memcpy(package + 7, (void *) ((char *) buffer + 7 + (8 * header.intern)), header.key_size);
        memcpy(package + 7 + header.key_size, value, value_size);
    }

    *result = (void *) package;
    return package_size;
}

unsigned int marshall_intern(void **result, void *buffer, void* value, unsigned short value_size,
    package_header header) {
    printf("############ marshall intern ##########\n");
    printf("ACK %d | GET %d | SET %d | DEL %d\n",header.ack,header.get,header.set,header.del);
    printf("############ marshall intern ##########\n");

    unsigned int package_size = 15 + header.key_size * (header.get || !header.ack)
        + value_size * (header.get || (!header.ack && header.set));
    char *package = calloc(package_size,1);
    unsigned long value_nb;
    unsigned short id_nb;
    uint32_t myip_nb;

    uint8_t byte = 128 + (8 * header.ack) + header.get * 4 + header.set * 2 + header.del;
    memcpy(package, &byte, 1);
    id_nb = htons(header.id);
    myip_nb = htonl(header.ip);
    memcpy(package + 7, &id_nb, 2);
    memcpy(package + 9, &myip_nb, 4);
    memcpy(package + 13, &header.port, 2);

    if (header.get || !header.ack) {
        memcpy(package + 1, (void *) ((char *) buffer + 1), 2);
        memcpy(package + 15, (void *) ((char *) buffer + 7 + (8 * header.intern)), header.key_size);
    }

    if (header.get || (!header.ack && header.set)) {
        value_nb = htonl(value_size);
        memcpy(package + 3, &value_nb, 4);
        memcpy(package + 15 + header.key_size, value, value_size);
    }

    *result = (void *) package;
    return package_size;
}

// unsigned int marshall_intern(void **result, void *buffer, void* value, unsigned short value_size,
//     package_header header) {
//     unsigned int package_size;
//     char *package;
//     if(header.intern && !header.ack){
//         package_size = 11;
//         package = calloc(package_size,1);
//         uint8_t byte = (1<<7) + 2 * header.lookup + 1 * header.reply;
//         memcpy(package, &byte, 1);
//         uint16_t hashID = htons(header.hashID);
//         memcpy(package + 1, &hashID,2);
//         uint16_t nodeID = htons(header.id);
//         memcpy(package + 3, &nodeID,2);
//         uint32_t nodeIP = htonl(header.ip);
//         memcpy(package + 5, &nodeIP,4);
//         uint16_t nodePORT = htons(header.port);
//         memcpy(package + 9, &nodePORT,2);
//     }
//     else {
//         package_size = 17 + header.key_size * (header.get || !header.ack)
//             + value_size * (header.get || (!header.ack && header.set));
//         package = calloc(package_size,1);
//         unsigned long value_nb;
//         uint32_t myip_nb;

//         uint8_t byte = 128 + (8 * header.ack) + header.get * 4 + header.set * 2 + header.del;
//         memcpy(package, &byte, 1);                                                      // write first header byte
//         value_nb = htonl(header.id);
//         myip_nb = htonl(header.ip);
//         memcpy(package + 3, &value_nb, 4);
//         memcpy(package + 11, &myip_nb, 4);
//         memcpy(package + 15, &header.port, 2);

//         if (header.get || !header.ack) {
//             memcpy(package + 1, (void *) ((char *) buffer + 1), 2);
//             memcpy(package + 17, (void *) ((char *) buffer + 7 + (10 * header.intern)), header.key_size);
//         }

//         if (header.get || (!header.ack && header.set)) {
//             value_nb = htonl(value_size);
//             memcpy(package + 3, &value_nb, 4);
//             memcpy(package + 17 + header.key_size, value, value_size);
//         }
//     }

//     *result = (void *) package;
//     return package_size;
// }

package_header unmarshall(void *buffer) {
    package_header header;

    uint8_t *byte = calloc(1,1);
    memcpy(byte, buffer, 1);
    header.intern = (*byte >> 7) & 0x1;
    header.ack = (*byte >> 3) & 0x1;
    header.get = (*byte >> 2) & 0x1;
    header.set = (*byte >> 1) & 0x1;
    header.del = *byte & 0x1;
    
    printf("############ unmarshall ##########\n");
    printf("ACK %d | GET %d | SET %d | DEL %d\n",header.ack,header.get,header.set,header.del);
    printf("############ unmarshall ##########\n");
    free(byte);

    unsigned int *size = calloc(sizeof(size),1);

    memcpy(size, (char *) buffer + 1, 2);
    header.key_size = ntohs(*size);

    memcpy(size, (char *) buffer + 3, 4);
    header.value_size = ntohl(*size);
    printf("unmarshalled k_s %d v_s %d\n",header.key_size,header.value_size);
    if (header.intern) {
        // memcpy(size, (char *) buffer + 1, 2);
        // header.hashID = ntohs(*size);
        // header.lookup = *byte & 0x1;
        // header.reply = (*byte >> 1) & 0x1;

        memcpy(size, (char *) buffer + 7, 2);
        header.id = ntohs(*size);

        uint32_t *ipaddr = calloc(sizeof(ipaddr),1);
        memcpy(ipaddr, (char *) buffer + 9, 4);
        header.ip = ntohl(*ipaddr);
        free(ipaddr);

        memcpy(size, (char *) buffer + 13, 2);
        header.port = *size;
    }
    // else if(header.intern && header.ack){
    //     memcpy(size, (char *) buffer + 1, 2);
    //     header.key_size = ntohs(*size);

    //     memcpy(size, (char *) buffer + 3, 4);
    //     header.value_size = ntohl(*size);

    //     memcpy(size, (char *) buffer + 7, 2);
    //     header.id = ntohs(*size);

    //     uint32_t *ipaddr = calloc(sizeof(ipaddr),1);
    //     memcpy(ipaddr, (char *) buffer + 9, 4);
    //     header.ip = ntohl(*ipaddr);
    //     free(ipaddr);

    //     memcpy(size, (char *) buffer + 13, 2);
    //     header.port = *size;
    // }

    free(size);
    return header;
}

void protokoll_debug(void *buffer, package_header header, const char* argv[]) {
    void *key, *value;
    char portstr[8];
    char ip4str[INET_ADDRSTRLEN];                       // space to hold the IPv4 string
    struct sockaddr_in sa;                              // pretend this is loaded with something

    printf("=======================\n");
    // printf("\nBuffer: %x\n", *(unsigned int *) buffer);
    printf("Intern: %d Ack: %d\n", header.intern, header.ack);
    printf("Get %d Set %d Del %d\n", header.get, header.set, header.del);
    printf("KeySize: %d ValueSize: %d\n", header.key_size, header.value_size);

    if (header.intern) {
        key = (void *) ((char *) buffer + 17);
        value = (void *) ((char *) buffer + 17 + header.key_size);
    } else {
        key = (void *) ((char *) buffer + 7);
        value = (void *) ((char *) buffer + 7 + header.key_size);
    }

    char *key_str = calloc((header.key_size + 1),1);
    memcpy(key_str, key, header.key_size);

    char *value_str = calloc((header.value_size + 1),1);
    memcpy(value_str, value, header.value_size);

    if (header.key_size) printf("Key: %s\n", (char *) key_str);
    if (header.value_size) printf("Value: %s\n", (char *) value_str);

    free(key_str);
    free(value_str);

    if (header.intern) {
        sa.sin_addr.s_addr = header.ip;
        snprintf(portstr, 7, "%d", ntohs(header.port));
        inet_ntop(AF_INET, &(sa.sin_addr), ip4str, INET_ADDRSTRLEN);
        printf("ID: %d IP: %s Port: %s\n", header.id, ip4str, portstr);
    }

    printf("preID: %s preIP: %s prePORT: %s\n", argv[4], argv[5], argv[6]);
    printf("sucID: %s sucIP: %s sucPORT: %s\n", argv[7], argv[8], argv[9]);
    printf("=======================\n");

    return;
}

#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include "protokoll.h"
#include "hashmap.h"

void new_elem(unsigned short hashID, int sock_fd){
    struct hash_client_elem * elem = malloc(sizeof(struct hash_client_elem));
    elem->hashID = hashID;
    elem->socket_fd = sock_fd;
    elem->next = NULL;
    printf("search for place \n");
    if(hc_header == NULL){
        printf("header is null \n");
        hc_header = elem;
    }else{
        struct hash_client_elem *temp = hc_header;
        printf("header is not null \n");
        while(temp->next != NULL && temp->hashID > 0){
            temp = temp->next;
        }
        temp->next = elem;
    }
    printf("added hash %d for %d\n",elem->hashID,elem->socket_fd);
}

 int get_sockfd(unsigned short hashID){
    printf("getting sockfd %d\n",hashID);
    if(hc_header == NULL) return 0;
    struct hash_client_elem * temp = hc_header;
    while(temp != NULL){
        if(temp->hashID == hashID)
            return temp->socket_fd;
        temp = temp->next;
    }
    return 0;
}

void delete_elem(unsigned short hashID, int sockfd){
    if(hc_header != NULL){
        struct hash_client_elem * current = hc_header;
        struct hash_client_elem * prev = NULL;
        while(current->hashID != hashID && current->socket_fd != sockfd){
            if(current->next == NULL){
                printf("tmp next == 0 !! didnt delete\n");
                return;
            }else{
                prev = current;
                current = current->next;
            }
        }
        if(current == hc_header){
            hc_header = hc_header->next;
        }else{
            prev->next = current->next;
        }
        current->hashID = 0;
        current->socket_fd = 0;
        free(current);
        current = NULL;
        printf("element deleted\n");
    }
}

unsigned int marshall_answer(void **result, void *buffer, void* value, unsigned int value_size,
    package_header header) {

    unsigned int package_size = 7 + (header.key_size + value_size) * header.get;
    char *package = calloc(package_size,1);
    unsigned int value_nb;

    uint8_t byte = 8 + header.get * 4 + header.set * 2 + header.del;
    memcpy(package, &byte, 1);                                                      // write first header byte

    if (value_size != 0 && value) {
        value_nb = htonl(value_size);
        memcpy(package + 1, (void *) ((char *) buffer + 1), 2);
        memcpy(package + 3, &value_nb, 4);
        memcpy(package + 7, (void *) ((char *) buffer + 7), header.key_size);
        memcpy(package + 7 + header.key_size, value, value_size);
    }

    *result = (void *) package;
    return package_size;
}

// marshall lookup/reply internal messages until we find the responsible successor
unsigned int marshall_internal_request(void **result, package_header header){
        unsigned int package_size = 11;
        char *package = calloc(package_size,1);
        unsigned short twobytes;
        uint32_t myip_nb;

        uint8_t byte = 128 + header.reply * 2 + header.lookup;
        memcpy(package, &byte, 1);                                                      // write first header byte

        twobytes = htons(header.hashID);
        memcpy(package + 1, &twobytes, 2);

        twobytes = htons(header.id);
        memcpy(package + 3, &twobytes, 2);

        myip_nb = htonl(header.ip);
        memcpy(package + 5, &myip_nb, 4);

        twobytes = htons(header.port);
        memcpy(package + 9, &twobytes, 2);
        
        *result = (void *) package;
        return package_size;
    }

unsigned int marshall_intern(void **result, void *buffer, void* value, unsigned int value_size, package_header header) {

    unsigned int package_size = 15 + header.key_size * (header.get || !header.ack)
        + value_size * (header.get || (!header.ack && header.set));
    char *package = calloc(package_size,1);
    unsigned short value_nb;
    uint32_t myip_nb;

    uint8_t byte = 128 + (8 * header.ack) + header.get * 4 + header.set * 2 + header.del;
    memcpy(package, &byte, 1);                                                      // write first header byte
    value_nb = htons(header.id);
    myip_nb = htonl(header.ip);
    memcpy(package + 7, &value_nb, 2);
    memcpy(package + 9, &myip_nb, 4);
    memcpy(package + 13, &header.port, 2);

    if (header.get || !header.ack) {
        memcpy(package + 1, (void *) ((char *) buffer + 1), 2);
        memcpy(package + 15, (void *) ((char *) buffer + 7 + (8 * header.intern)), header.key_size);
    }

    if (header.get || (!header.ack && header.set)) {
        unsigned int value_nb_l = htonl(value_size);
        memcpy(package + 3, &value_nb_l, 4);
        memcpy(package + 15 + header.key_size, value, value_size);
    }
    *result = (void *) package;
    return package_size;
}

package_header unmarshall(void *buffer) {
    package_header header;

    uint8_t *byte = calloc(1,1);
    memcpy(byte, buffer, 1);
    header.intern = (*byte >> 7) & 0x1;

    // memcpy(byte, (char *) buffer + 1, 2);
    // header.hashID = ntohs(*byte);

    unsigned int *size = calloc(sizeof(size),1);

    if (header.intern) {
        header.reply = (*byte >>1) & 0x1;
        header.lookup = *byte & 0x1;

        memcpy(size, (char *) buffer + 1, 2);
        header.hashID = ntohs(*size);

        memcpy(size, (char *) buffer + 3, 2);
        header.id = ntohs(*size);

        uint32_t *ipaddr = calloc(sizeof(ipaddr),1);
        memcpy(ipaddr, (char *) buffer + 5, 4);
        header.ip = ntohl(*ipaddr);
        free(ipaddr);

        memcpy(size, (char *) buffer + 9, 2);
        header.port = *size;
    }else{
        header.ack = (*byte >> 3) & 0x1;
        header.get = (*byte >> 2) & 0x1;
        header.set = (*byte >> 1) & 0x1;
        header.del = *byte & 0x1;

        memcpy(size, (char *) buffer + 1, 2);
        header.key_size = ntohs(*size);

        memcpy(size, (char *) buffer + 3, 4);
        header.value_size = ntohl(*size);
        printf("unmarshalled vs %d\n",header.value_size);
    }
    free(byte);

    free(size);
    return header;
}

void protokoll_debug(void *buffer, package_header header, const char* argv[]) {
    void *key, *value;
    // char portstr[8];
    char ip4str[INET_ADDRSTRLEN];                       // space to hold the IPv4 string
    struct sockaddr_in sa;              

    printf("=======================\n");

    if (!header.intern) {
        key = (void *) ((char *) buffer + 7);
        value = (void *) ((char *) buffer + 7 + header.key_size);
        char *key_str = calloc(header.key_size + 1,1);
        memcpy(key_str, key, header.key_size);

        char *value_str = calloc(header.value_size + 1,1);
        memcpy(value_str, value, header.value_size);

        printf("Intern: %d Ack: %d Get %d Set %d Del %d\n", header.intern, header.ack,header.get, header.set, header.del);
        printf("KeySize: %d ValueSize: %d\n", header.key_size, header.value_size);
        if (header.key_size) printf("Key: %s\n", (char *) key_str);
        if (header.value_size) printf("Value: %s\n", (char *) value_str);

        free(key_str);
        free(value_str);
    }
    else if (header.intern) {
        sa.sin_addr.s_addr = header.ip;
        inet_ntop(AF_INET, &(sa.sin_addr), ip4str, INET_ADDRSTRLEN);
        printf("Intern: %d Reply: %d Lookup %d\n", header.intern, header.reply,header.lookup);
        printf("HASHID: %d ID: %d IP: %s Port: %d\n", header.hashID, header.id, ip4str, header.port);
    }

    printf("preID: %s preIP: %s prePORT: %s\n", argv[4], argv[5], argv[6]);
    printf("sucID: %s sucIP: %s sucPORT: %s\n", argv[7], argv[8], argv[9]);
    printf("=======================\n");

    return;
}

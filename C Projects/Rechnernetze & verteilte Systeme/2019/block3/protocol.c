#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include "protocol.h"

#define ACK_BIT (1<<3)
#define GET_BIT (1<<2)
#define SET_BIT (1<<1)
#define DEL_BIT (1)

void print_report(void *buffer, package_header header) {
    void *key = (void *) ((unsigned char *) buffer + 7);

    char *key_str = calloc(header.key_length + 1 * sizeof(char),1);
    memcpy(key_str, key, header.key_length);

    printf("############### protokoll debug ################\n");
    printf("GET %d\n", (header.rcp_cmd >> 2) & 0x1);
    printf("SET %d\n", (header.rcp_cmd >> 1) & 0x1);
    printf("DELETE %d\n", header.rcp_cmd & 0x1);
    printf("Key Size: %d\n", header.key_length);
    printf("Value Size: %d\n", header.value_length);
    printf("Key: %s\n", (char *) key_str);

    free(key_str);
    return;
}

unsigned int marshall(void **result, void *buffer, void* value, unsigned int value_length, package_header header) {
    unsigned int package_size = 7 + header.key_length + (value_length) * header.rcp_cmd;
    uint16_t key_length_nb;
    uint32_t value_length_nb;
    char *package = calloc(package_size,1);

    header.rcp_cmd |= ACK_BIT;
    memcpy(package, &header.rcp_cmd, 1);

    if (value_length != 0 && value) {
        value_length_nb = htonl(value_length);
        key_length_nb = htons(header.key_length);
        memcpy(package + 1, &key_length_nb, 2);
        memcpy(package + 3, &value_length_nb, 4);
        memcpy(package + 7, (void *) ((char *) buffer + 6), header.key_length);
        memcpy(package + 7 + header.key_length, value, value_length);
    }

    *result = (void *) package;
    return package_size;
}

package_header unmarshall(void *buffer) {
    package_header header;

    uint8_t *byte = calloc(1,1);
    memcpy(byte, buffer, 1);
    header.rcp_cmd = byte[0];

    size_t *k_size = calloc(sizeof(k_size),1);
    memcpy(k_size, (char *) buffer + 1, 2);
    header.key_length = ntohs(*k_size);

    size_t *v_size = calloc(sizeof(v_size),1);
    memcpy(v_size, (char *) buffer + 3, 4);

    header.value_length = ntohl(*v_size);
    free(k_size);
    free(v_size);
    free(byte);

    return header;
}

#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "protokoll.h"

#define ACK_BIT (1<<3)
#define GET_BIT (1<<2)
#define SET_BIT (1<<1)
#define DEL_BIT (1)

void print_report(void *buffer, package_header header) {
    void *key = (void *) ((unsigned char *) buffer + 7);
    void *value = (void *) ((unsigned char *) buffer + 7 + header.key_length);

    char *key_str = calloc(header.key_length + 1 * sizeof(char),1);
    memcpy(key_str, key, header.key_length);
    char *val_str = calloc(header.value_length + 1 * sizeof(char),1);
    memcpy(val_str, value, header.value_length);

    fprintf(stderr,"############### protokoll debug ################\n");
    fprintf(stderr,"ACK %d\n", (header.rcp_cmd >> 3) & 0x1);
    fprintf(stderr,"GET %d\n", (header.rcp_cmd >> 2) & 0x1);
    fprintf(stderr,"SET %d\n", (header.rcp_cmd >> 1) & 0x1);
    fprintf(stderr,"DELETE %d\n", header.rcp_cmd & 0x1);
    fprintf(stderr,"Key Size: %d\n", header.key_length);
    fprintf(stderr,"Value Size: %d\n", header.value_length);
    fprintf(stderr,"Key: %s\n", (char *) key_str);
    fprintf(stderr,"Value: %s\n", (char *) key_str);

    free(key_str);
    return;
}

unsigned int marshall_query(void **result, uint8_t operation, const char *key, const char *value) {
    unsigned int value_length;
    unsigned int key_length = strlen(key);

    char *vvalue; 
    if(operation == 2){
        vvalue = malloc(4294967295);
        value_length = read(STDIN_FILENO, vvalue, 4294967295);
    } 
    else value_length = value ? strlen(value) : 0;
    unsigned short key_length_nb = htons(key_length);
    unsigned int value_length_nb = htonl(value_length);
    unsigned int package_size = 7 + key_length + value_length;
    char *package = calloc(package_size,1);

    memcpy(package, &operation, 1);
    memcpy(package + 1, &key_length_nb, 2);
    memcpy(package + 3, &value_length_nb, 4);
    memcpy(package + 7, key, key_length);
    if (operation == 2) memcpy(package + 7 + key_length, vvalue, value_length);  
    else                memcpy(package + 7 + key_length, value, value_length);

    *result = (void *) package;
    return package_size;
}

unsigned int marshall_answer(void **result, void *buffer, void* value, unsigned int value_length, package_header header) {
    unsigned int package_size = 6 + (header.key_length + value_length) * ((header.rcp_cmd & GET_BIT)>>2);
    unsigned int value_length_nb;
    char *package = calloc(package_size,1);

    uint8_t byte = 8 + header.rcp_cmd;
    memcpy(package, &byte, 1);                                                      // write first header byte

    if (value_length != 0 && value) {
        value_length_nb = htonl(value_length);
        memcpy(package + 1, (void *) ((char *) buffer + 2), 2);
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
    fprintf(stderr,"k_size %d\n",header.key_length);

    size_t *v_size = calloc(sizeof(v_size),1);
    memcpy(v_size, (char *) buffer + 3, 4);
    header.value_length = ntohl(*v_size);
    fprintf(stderr,"v_size %d\n",header.value_length);

    free(k_size);
    free(v_size);
    free(byte);

    return header;
}


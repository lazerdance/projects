#ifndef protocol_h
#define protocol_h

typedef struct header_struct {
    uint8_t rcp_cmd;
    uint16_t key_length;
    uint32_t value_length;
} package_header;

void print_report(void *buffer, package_header head);
unsigned int marshall(void **result, void *buffer, void* value, unsigned int value_length, package_header header);
package_header unmarshall(void *buffer);

#endif

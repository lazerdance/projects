#ifndef protokoll_h
#define protokoll_h

struct hash_client_elem{
    unsigned short hashID;
    int socket_fd;
    struct hash_client_elem *next;
};

struct hash_client_elem *hc_header;
void new_elem(unsigned short hashID, int sock_fd);
int get_sockfd(unsigned short hashID);
void delete_elem(unsigned short hashID, int sockfd);

typedef struct _package_header {
	uint8_t intern;                 // control bit

    uint8_t ack;
    uint8_t get;
    uint8_t set;
    uint8_t del;
    unsigned int key_size;
    unsigned int value_size;

    uint8_t reply;
    uint8_t lookup;
    uint16_t hashID;
    unsigned short id;
    unsigned short port;
    uint32_t ip;
} package_header;

unsigned int marshall_answer(void **result, void *buffer, void* value, unsigned int value_size,
    package_header header);
unsigned int marshall_internal_request(void **result, package_header header);
unsigned int marshall_intern(void **result, void *buffer, void* value, unsigned int value_size,
    package_header header);

package_header unmarshall(void *buffer);
void protokoll_debug(void *buffer, package_header head, const char* argv[]);

#endif

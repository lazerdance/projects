#ifndef connection_h
#define connection_h

int listen_to(const char* dns, const char* port, uint32_t *myip, unsigned short *myport);
int connect_to(const char *dns, const char *port);
int receive_data(int socket_fd, void **datapointer);
int send_data(int socket_fd, void *data, unsigned int data_size);
void translate_ip_and_port(const char* dns, const char* portstr, uint32_t *ip4, unsigned short *port);

#endif
/*
    Rechnernetze und Verteilte Systeme - Block 3; Aufgabe 6;
    Gruppe: T18G01
*/

#ifndef connection_h
#define connection_h

void pass_host(const char *dns_p, const char *port_p);
void free_host();
void close_connection();
int setup_connection();
int receive_data(void **datapointer, struct timeval *tv);
int send_package(void *data, unsigned int data_size);

#endif

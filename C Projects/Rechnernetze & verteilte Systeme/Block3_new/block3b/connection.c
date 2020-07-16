#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include "protokoll.h"

#define BUFSIZE 512

int socket_fd;
char *dns;
char *port;

void pass_host(const char *dns_p, const char *port_p) {
    dns = calloc((strlen(dns_p) + 1) * sizeof(char),1);
    port = calloc((strlen(port_p) + 1) * sizeof(char),1);
    strncpy(dns, dns_p, strlen(dns_p));
    strncpy(port, port_p, strlen(port_p));
}

void free_host() {
    free(dns);
    free(port);
}

void close_connection() {
    close(socket_fd);
}

int setup_connection() {
    int status;                                 // rueckgabewert von getaddrinfo()
    struct addrinfo hints;                      // addrinfo um verbindungsparameter fetzulegen
    struct addrinfo *p, *servinfo;              // addrinfo liste fuer rueckgabe von getaddrinfo
    memset(&hints, 0, sizeof (hints));

    // Vorgaben fuer getaddrinfo
    hints.ai_family = AF_UNSPEC;                // IPv4 oder IPv6
    hints.ai_socktype = SOCK_STREAM;            // Verwende TCP

    // getaddrinfo call + fehlerbehandlung
    if ((status = getaddrinfo(dns, port, &hints, &servinfo)) != 0) {
        printf("getaddrinfo error: %s\n", gai_strerror(status));
        return 0;
    }

    // iteriere durch die rueckgaben von getaddrinfo
    for (p = servinfo; p != NULL; p = p->ai_next) {
        // probiere socket mit den daten von getaddrinfo zu deklarieren
        if ((socket_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            // printf("Kann kein socket erstellen... probiere weiter...\n");
            continue;
        }

        // probiere ueber socket zu verbinden
        if (connect(socket_fd, p->ai_addr, p->ai_addrlen) == -1) {
            // printf("Kann sich nicht Verbinden... probiere weiter...\n");
            close(socket_fd);
            continue;
        }
        break;                              // wenn verbunden, breche die schleife ab
    }

    // Pruefe ob eine Verbindung hergestellt wurde.
    if (p == NULL) {
        freeaddrinfo(servinfo);
        return 0;
    }

    freeaddrinfo(servinfo);
    return 1;
}

int receive_data(void **datapointer, struct timeval *tv) {
    int bytes_recv;
    int bytes_total = 0;
    int package_size = 0;
    package_header header;
    fd_set readfds;
    // buffer fuer die empfangenen Daten
    void *data ;
    void *buffer = calloc(BUFSIZE,1);

    FD_ZERO(&readfds);
    FD_SET(socket_fd, &readfds);
    select(socket_fd + 1, &readfds, NULL, NULL, tv ? tv : NULL);
    if (FD_ISSET(socket_fd, &readfds)) {
        while ((bytes_recv = recv(socket_fd, buffer, BUFSIZE, 0)) != 0) {
            if (bytes_recv == -1) {
                free(data);
                free(buffer);
                return -1;
            }

            bytes_total += bytes_recv;
            if (!package_size) {
                header = unmarshall(buffer);
                package_size = 7 + header.key_length + header.value_length;
                fprintf(stderr,"malloc %d\n",package_size);
                data = malloc(package_size);
            }
            // data = realloc(data, (bytes_total + 1));
            if (data == NULL) {
                printf("Can't allocate more memory. Exiting.\n");
                exit(1);
            }
            
            fprintf(stderr,"loop %d\n",bytes_total - bytes_recv);            
            memcpy((char *) data + bytes_total - bytes_recv, buffer, bytes_recv);
            fprintf(stderr,"received %d\n",bytes_total);
            if (package_size >= bytes_total) break;
        }
    } else {
        // free(data);
        free(buffer);
        return -2;
    }
    fprintf(stderr,"bytes total %d\n",bytes_total);

    free(buffer);
    *datapointer = data;
    return bytes_total;
}

int send_package(void *data, unsigned int data_size) {
    int total = 0;                 
    int sended = 0;
    int bytesleft = data_size;     

    while(total < (int) data_size) {
        sended = send(socket_fd, (void*) (((char *) data) + total), bytesleft, 0);
        if (sended == -1) return -1;
        total += sended;
        bytesleft -= sended;
    }

    return total;
}

/*
    Rechnernetze und Verteilte Systeme - Block 3; Aufgabe 6;
    Gruppe: T18G01
*/

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#include "memory.h"
#include "protokoll.h"

#define BUFSIZE 512                         // groesse buffer

int socket_fd;                              // socket file discriptor
char *dns;
char *port;

void pass_host(const char *dns_p, const char *port_p) {
    dns = myalloc((strlen(dns_p) + 1) * sizeof(char));
    port = myalloc((strlen(port_p) + 1) * sizeof(char));
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
    int bytes_recv;                         // rueckgabewert von receive: bytes die empfangen wurden
    int bytes_total = 0;
    int package_size = 0;
    package_header header;
    fd_set readfds;
    // buffer fuer die empfangenen Daten
    void *data = myalloc(1);
    void *buffer = myalloc(BUFSIZE);

    FD_ZERO(&readfds);
    FD_SET(socket_fd, &readfds);
    select(socket_fd + 1, &readfds, NULL, NULL, tv ? tv : NULL);

    if (FD_ISSET(socket_fd, &readfds)) {
        // probiere Daten zu empfangen
        while ((bytes_recv = recv(socket_fd, buffer, BUFSIZE, 0)) != 0) {
            // fehlerbehandlung bei der Datenuebertragung
            if (bytes_recv == -1) {
                free(data);
                free(buffer);
                return -1;
            }

            bytes_total += bytes_recv;
            data = realloc(data, (bytes_total + 1));
            if (data == NULL) {
                printf("Can't allocate more memory. Exiting.\n");
                exit(1);
            }

            memcpy((char *) data + bytes_total - bytes_recv, buffer, bytes_recv);

            if (!package_size) {
                header = unmarshall(data);
                package_size = (signed int) (6 + header.key_size + header.value_size);
            }
            if (package_size == bytes_total) break;
        }
    } else {
        free(data);
        free(buffer);
        return -2;
    }

    free(buffer);
    *datapointer = data;
    return bytes_total;
}

int send_package(void *data, unsigned int data_size) {
    int total = 0;                  // how many bytes we've sent
    int sended = 0;
    int bytesleft = data_size;      // how many we have left to send

    while(total < (int) data_size) {
        sended = send(socket_fd, (void*) (((char *) data) + total), bytesleft, 0);
        if (sended == -1) return -1;
        total += sended;
        bytesleft -= sended;
    }

    return total;
}

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include "protokoll.h"

#define BACKLOG 16
#define BUFSIZE 512

int listen_to(const char* dns, const char* port, uint32_t *myip, unsigned short *myport) {
    int yes = 1;                            // Wert fuer setsockopt()
    int rv, listener;                       // return value; socket file discriptors
    struct addrinfo hints;                  // addrinfo um verbindungsparameter fetzulegen
    struct addrinfo *p;                     // addrinfo pointer um ueber liste fuer rueckgabe zu iterieren
    struct addrinfo *servinfo;              // addrinfo liste fuer rueckgabe von getaddrinfo

    memset(&hints, 0, sizeof (hints));
    hints.ai_family = AF_INET;              // IPv4
    hints.ai_socktype = SOCK_STREAM;        // Verwende TCP
    // hints.ai_flags = AI_PASSIVE;            // eigene IP automatisch eintragen

    // getaddrinfo call + fehlerbehandlung
    if ((rv = getaddrinfo(dns, port, &hints, &servinfo)) != 0) {
        printf("getaddrinfo error: %s\n", gai_strerror(rv));
        exit(1);
    }

    // iteriere durch die rueckgaben von getaddrinfo
    for (p = servinfo; p != NULL; p = p->ai_next) {
        // probiere socket mit den daten von getaddrinfo zu deklarieren
        if ((listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            printf("Kann kein socket erstellen... probiere weiter...\n");
            continue;
        }

        // Port nur blockieren, wenn auf dem Port aktiv gelauscht wird
        if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
            printf("Kann Port Optionen nicht setzen.\n");
            freeaddrinfo(servinfo);             // speicher freigeben
            close(listener);                   // socket schliessen
            exit(1);
        }

        // versuche socket an Port zu binden
        if (bind(listener, p->ai_addr, p->ai_addrlen) == -1) {
            printf("Kann socket nicht an den Port binden... probiere weiter...\n");
            close(listener);
            continue;
        }

        break;                              // wenn verbunden, breche die schleife ab
    }

    // Pruefe ob der Port gebunden wurde
    if (p == NULL) {
        printf("Kann Port nicht binden.\n");
        freeaddrinfo(servinfo);
        exit(1);
    }

    *myip = ((struct sockaddr_in *) p->ai_addr)->sin_addr.s_addr;
    *myport = ((struct sockaddr_in *) p->ai_addr)->sin_port;

    freeaddrinfo(servinfo);                 // speicher freigeben

    // fange an auf dem port zu lauschen; maximal 5 remotes gleichzeitig
    if (listen(listener, BACKLOG) == -1) {
        printf("Kann auf dem Port nicht hoeren.\n");
        close(listener);                   // socket schliessen
        exit(1);
    }

    return listener;
}

int connect_to(const char *dns, const char *port) {
    int status;                                 // rueckgabewert von getaddrinfo()
    int socket_fd;
    struct addrinfo hints;                      // addrinfo um verbindungsparameter fetzulegen
    struct addrinfo *p, *servinfo;              // addrinfo liste fuer rueckgabe von getaddrinfo
    memset(&hints, 0, sizeof (hints));

    // Vorgaben fuer getaddrinfo
    hints.ai_family = AF_INET;                // IPv4 oder IPv6
    hints.ai_socktype = SOCK_STREAM;            // Verwende TCP

    // getaddrinfo call + fehlerbehandlung
    if ((status = getaddrinfo(dns, port, &hints, &servinfo)) != 0) {
        printf("getaddrinfo error: %s\n", gai_strerror(status));
        return -1;
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
        return -1;
    }

    freeaddrinfo(servinfo);
    return socket_fd;
}

int receive_data(int socket_fd, void **datapointer) {
    int bytes_recv;                            // rueckgabewert von receive: bytes die empfangen wurden
    int bytes_total = 0;
    int package_size = 0;
    package_header header;
    // buffer fuer die empfangenen Daten
    void *data = calloc(1,1);
    void *buffer = calloc(BUFSIZE,1);

    // probiere Daten zu empfangen
    while ((bytes_recv = recv(socket_fd, buffer, BUFSIZE, 0)) != 0) {
        // fehlerbehandlung bei der Datenuebertragung
        if (bytes_recv == -1) {
            free(data);
            free(buffer);
            return -1;
        }
        printf("reading\n");
        bytes_total += bytes_recv;
        data = realloc(data, (bytes_total + 1));
        if (data == NULL) {
            printf("Can't allocate more memory. Exiting.\n");
            exit(1);
        }

        memcpy((char *) data + bytes_total - bytes_recv, buffer, bytes_recv);

        if (!package_size) {
            header = unmarshall(data);
            package_size = (signed int) (6 + header.key_size + header.value_size + (header.intern * 8));
        }
        if (package_size <= bytes_total) break;
    }
    free(buffer);
    *datapointer = data;

    if (bytes_total == 0) {
        *datapointer = NULL;
        free(data);
    }

    return bytes_total;
}

int send_data(int socket_fd, void *data, unsigned int data_size) {
    int total = 0;                  // how many bytes we've sent
    int sended = 0;
    int bytesleft = data_size;      // how many we have left to send
    unsigned int *val_buf = calloc(sizeof(val_buf),1);
    memcpy(val_buf,((char *) data) + 3, 4);
    uint32_t value_nb = ntohl(*val_buf);
    printf("value before send %u\n",value_nb);
    while(total < (int) data_size) {
        sended = send(socket_fd, (void*) (((char *) data) + total), bytesleft, 0);
        printf("total %d | sent %d\n",total,sended);
        if (sended == -1) return -1;
        total += sended;
        bytesleft -= sended;
    }

    return total;
}

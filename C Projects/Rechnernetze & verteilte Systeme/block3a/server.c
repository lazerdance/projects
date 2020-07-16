/*
    Rechnernetze und Verteilte Systeme - Block 3; Aufgabe 5;
    Gruppe: T18G01
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <signal.h>

#include "memory.h"
#include "hashmap.h"
#include "protokoll.h"

#define BACKLOG 5                           // maximale verbindungen die auf accept() warten
#define HASHSIZE 64
#define BUFSIZE 512                         // maximale quote laenge

// globale Variable um Socket ordentlich freizugeben
int socket_fd;                              // socket file discriptor
hash_map hashmap;

void *myalloc(const unsigned int size) {
    void *memory = calloc(size, 1);
    if (!memory) {
        printf("Can't allocate more memory. Exiting.\n");
        exit(1);
    }
    return memory;
}

void intHandler() {
    free_hashmap(hashmap);                  // free hashmap
    close(socket_fd);                       // socket schliessen
    // printf("\n");
    exit(0);
    return;
}

int bind_socket(struct addrinfo *servinfo) {
    int f_socket = -1;
    int const yes = 1;                      // Wert fuer setsockopt()
    struct addrinfo *p;                     // addrinfo pointer um ueber liste fuer rueckgabe zu iterieren

    // iteriere durch die rueckgaben von getaddrinfo
    for (p = servinfo; p != NULL; p = p->ai_next) {
        // probiere socket mit den daten von getaddrinfo zu deklarieren
        if ((f_socket = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            printf("Kann kein socket erstellen... probiere weiter...\n");
            continue;
        }

        // Port nur blockieren, wenn auf dem Port aktiv gelauscht wird
        if (setsockopt(f_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            printf("Kann Port Optionen nicht setzen.\n");
            freeaddrinfo(servinfo);             // speicher freigeben
            close(f_socket);                   // socket schliessen
            exit(1);
        }

        // versuche socket an Port zu binden
        if (bind(f_socket, p->ai_addr, p->ai_addrlen) == -1) {
            printf("Kann socket nicht an den Port binden.\n");
            close(f_socket);
            continue;
        }

        break;                              // wenn verbunden, breche die schleife ab
    }

    // Pruefe ob socket mit gebundenen Port erstellt wurde
    if (p == NULL) {
        return -1;
    }

    return f_socket;
}

int receive_data(int socket_fd, void **datapointer) {
    int bytes_recv;                            // rueckgabewert von receive: bytes die empfangen wurden
    int bytes_total = 0;
    int package_size = 0;
    package_header header;
    // buffer fuer die empfangenen Daten
    void *data = myalloc(1);
    void *buffer = myalloc(BUFSIZE);

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
    free(buffer);

    *datapointer = data;
    return bytes_total;
}

int send_package(int connect_socket, void *data, unsigned int data_size) {
    int status = send(connect_socket, data, data_size, 0);
    if (status == -1) {
        printf("Konnte Daten nicht uebertragen.\n");
    } else if (status != (int) data_size) {
        printf("Konnte nicht die gesamte Nachricht verschicken.\n");
    }
    return status;
}

void handle_connection(int connect_socket) {
        int bytes_recv;                     // rueckgabewert von receive: bytes die empfangen wurden
        unsigned int package_size;          // groeße der Antwort
        void *key, *value, *package, *buffer;
        package_header header;
        hash_entry *hash_data = NULL;

        // probiere Daten zu empfangen
        while ((bytes_recv = receive_data(connect_socket, &buffer)) != 0) {
            if (bytes_recv == -1) {
                printf("Error while receiving data.\n");
                return;
            }

            // entpacke empfangene Daten
            header = unmarshall(buffer);
            protokoll_debug(buffer, header);

            key = myalloc(header.key_size);             // neuer Speicher für die Hashmap; buffer wird freigegeben
            memcpy(key, (void *) ((char *) buffer + 6), header.key_size);

            // Aufruf der Hashmap
            if (header.get) {
                if(!(hash_data = get(hashmap, key, header.key_size))) header.get = 0;
            } else if (header.set) {
                value = myalloc(header.value_size);     // neuer Speicher für die Hashmap; buffer wird freigegeben
                memcpy(value, (void *) ((char *) buffer + 6 + header.key_size), header.value_size);
                set(hashmap, key, header.key_size, value, header.value_size);
            } else if (header.del) {
                if(!del(hashmap, key, header.key_size)) header.del = 0;
            } else {
                printf("No Action Bit Set\n");
                free(key);
                free(buffer);
                return;
            }

            if (hash_data) package_size = marshall(&package, buffer, hash_data->value, hash_data->value_size, header);
            else package_size = marshall(&package, buffer, NULL, 0, header);

            send_package(connect_socket, package, package_size);

            hash_data = NULL;
            free(package);
            free(buffer);
        }

        free(buffer);
        return;
}

int main(const int argc, const char* argv[]) {
    // Kontrolliere ob Eingabeparameter vorhanden sind
    if (argc != 2) {
        printf("Das Programm sollte wie folgt aufgerufen werden:\n./program port\n");
        exit(1);
    }

    // Kontrolliere ob Port in Ordnung ist
    long port = atol(argv[1]);
    if(port < 0 || port > 65535) {
        perror("Bitte einen Port zwischen 0 und 65535 angeben.");
        exit(1);
    }

    // Variablen deklarieren
    int status;                             // rueckgabewert von getaddrinfo() oder gesendete bits
    int connect_socket;                     // socket fuer verbindung zum remote
    struct addrinfo hints;                  // addrinfo um verbindungsparameter fetzulegen
    struct addrinfo *servinfo;              // addrinfo liste fuer rueckgabe von getaddrinfo
    struct sockaddr_storage remote_addr;    // adressen vom remote
    socklen_t sin_size;                     // groessen von adressen von remote

    // Variablen initieren
    hashmap = create_hashmap(HASHSIZE);
    sin_size = sizeof(remote_addr);
    memset(&hints, 0, sizeof (hints));

    hints.ai_family = AF_UNSPEC;            // IPv4 oder IPv6
    hints.ai_socktype = SOCK_STREAM;        // Verwende TCP
    hints.ai_flags = AI_PASSIVE;            // eigene IP automatisch eintragen

    // getaddrinfo call + fehlerbehandlung
    if ((status = getaddrinfo(NULL, argv[1], &hints, &servinfo)) != 0) {
        printf("getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }

    // Pruefe ob socket mit gebundenen Port erstellt wurde
    if ((socket_fd = bind_socket(servinfo)) == -1) {
        printf("Kann kein Socket mit gebundenen Port erstellen.\n");
        freeaddrinfo(servinfo);                 // speicher freigeben
        exit(1);
    }

    freeaddrinfo(servinfo);                 // speicher freigeben
    signal(SIGINT, intHandler);             // Signal handler erstellen

    // fange an auf dem port zu lauschen; maximal 5 remotes gleichzeitig
    if (listen(socket_fd, BACKLOG) == -1) {
        printf("Kann auf dem Port nicht hoeren.\n");
        close(socket_fd);                   // socket schliessen
        exit(1);
    }

    // dauerschleife um immer wieder nachricht zu verschicken
    while (1) {
        // warte auf Verbindung und akzeptiere
        if ((connect_socket = accept(socket_fd, (struct sockaddr*) &remote_addr, &sin_size)) == -1) {
            printf("Accept Error\n");
            continue;
        }

        handle_connection(connect_socket);
        close(connect_socket);              // remote socket freigeben
    }

    free_hashmap(hashmap);
    close(socket_fd);
    return 0;
}

/*
    Rechnernetze und Verteilte Systeme - Block 5; Aufgabe 4;
    Gruppe: T18G01
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <signal.h>
#include <arpa/inet.h>

#include "list.h"
#include "memory.h"
#include "hashmap.h"
#include "protokoll.h"
#include "connection.h"

#ifndef HASHSIZE
#define HASHSIZE 65536
#endif

// ----------------------------------------------------------------------------------------------------------------- //

int nodeID, preID, sucID;
char prePort[8], sucPort[8], preIP[INET_ADDRSTRLEN + 1], sucIP[INET_ADDRSTRLEN + 1];

int fdmax, listener;
fd_set master, read_fds;

uint32_t myip;
unsigned short myport;

qhead transid_list;
hash_map hashmap;

// ----------------------------------------------------------------------------------------------------------------- //

void *myalloc(const unsigned int size) {
    void *memory = calloc(size, 1);
    if (!memory) {
        printf("Can't allocate more memory. Exiting.\n");
        exit(1);
    }
    return memory;
}

void intHandler() {
    for (int i = 0; i <= fdmax; i++) {
        if (FD_ISSET(i, &master)) close(i);
    }
    free_list(&transid_list);
    if (hashmap.size) free_hashmap(hashmap);
    exit(0);
}

// ----------------------------------------------------------------------------------------------------------------- //

void completed_request(package_header header, void* buf) {
    int *rv, newfd, nbytes, status;
    void *package;

    rv = find_list(&transid_list, header.transid);
    if (!rv) {
        printf("Client disconnected? Can't find open Transaction.\n");
        free(buf);
        return;
    }
    newfd = rv[0];
    header.transid = rv[1];
    free(rv);
    remove_transid_list(&transid_list, header.transid);

    nbytes = marshall_answer(&package, buf, (void *) ((char *) buf + 14 + header.key_size),
        header.value_size, header);
    status = send_data(newfd, package, nbytes);
    if (status == -1) printf("Can't answer client.\n");
    free(package);
    free(buf);
    return;
}

void open_request_forward(int socketfd, package_header header, void* buf) {
    void *package;
    uint8_t transFinder;
    int newfd, status, nbytes, *rv;

    newfd = connect_to(sucIP, sucPort);
    if (newfd == -1) {
        printf("Can't connect to next peer.\n");
        free(buf);
        intHandler();
    }
    // CASE 2a NACHRICHT VON ANDEREM PEER
    if (header.intern) {
        nbytes = 14 + header.key_size + header.value_size;
        status = send_data(newfd, buf, nbytes);
        if (status == -1) printf("Can't forward to Peer.\n");
    }
    // CASE 2b NACHRICHT VON CLIENT
    else {
        transFinder = header.transid;
        rv = find_list(&transid_list, header.transid);
        while (rv) {
            transFinder++;
            free(rv);
            rv = find_list(&transid_list, transFinder);
        }
        append_list(&transid_list, socketfd, transFinder, header.transid);
        header.transid = transFinder;
        header.id = nodeID;
        header.ip = myip;
        header.port = myport;
        nbytes = marshall_intern(&package, buf,
            (void *) ((char *) buf + 6 + header.key_size), header.value_size, header);
        status = send_data(newfd, package, nbytes);
        if (status == -1) printf("Can't forward to Peer.\n");
        free(package);
    }
    close(newfd);
    return;
}

void open_request_response_peer(hash_entry* hash_data, package_header header, void* buf) {
    int nbytes, newfd, status;                          // socket file descriptors
    void *package;
    char portstr[8];
    char ip4str[INET_ADDRSTRLEN + 1];                   // space to hold the IPv4 string
    struct sockaddr_in sa;                              // pretend this is loaded with something

    snprintf(portstr, 7, "%d", ntohs(header.port));
    sa.sin_addr.s_addr = header.ip;
    inet_ntop(AF_INET, &(sa.sin_addr), ip4str, INET_ADDRSTRLEN);

    header.ack = 1;
    header.id = nodeID;
    header.ip = myip;
    header.port = myport;

    newfd = connect_to(ip4str, portstr);
    if (newfd == -1) {
        printf("Can't connect to a peer.\n");
        free(buf);
        intHandler();
    }

    if (hash_data) nbytes = marshall_intern(&package, buf, hash_data->value,
        hash_data->value_size, header);
    else nbytes = marshall_intern(&package, buf, NULL, 0, header);

    status = send_data(newfd, package, nbytes);
    if (status == -1) printf("Can't answer Peer.\n");
    close(newfd);
    free(package);
    return;
}

void open_request_response_direct(hash_entry* hash_data, int socketfd, package_header header, void* buf) {
    int nbytes, status;                // socket file descriptors
    void *package;

    if (hash_data) nbytes = marshall_answer(&package, buf, hash_data->value,
        hash_data->value_size, header);
    else nbytes = marshall_answer(&package, buf, NULL, 0, header);
    status = send_data(socketfd, package, nbytes);
    if (status == -1) printf("Can't answer Client.\n");
    free(package);
    return;
}

void open_request_response(int socketfd, package_header header, void* buf) {
    int hashrange;
    void *key, *value;
    hash_entry *hash_data = NULL;

    if (header.set && hashmap.size == 0) {
        if (nodeID < preID) hashrange = nodeID + HASHSIZE - preID;
        else if (nodeID == preID) hashrange = HASHSIZE;
        else hashrange = nodeID - preID;
        hashmap = create_hashmap(hashrange);
        printf("init hashmap of peer %d with hashsize: %d\n", nodeID, hashrange);
    }

    key = myalloc(header.key_size);
    memcpy(key, (void *) ((char *) buf + 6 + (8 * header.intern)), header.key_size);

    if (hashmap.size != 0) {
    // Aufruf der Hashmap
        if (header.get) {
            if(!(hash_data = get(hashmap, key, header.key_size))) header.get = 0;
        } else if (header.set) {
            value = myalloc(header.value_size);
            memcpy(value, (void *) ((char *) buf + 6 + (8 * header.intern) + header.key_size),
                header.value_size);
            set(hashmap, key, header.key_size, value, header.value_size);
        } else if (header.del) {
            if(!del(hashmap, key, header.key_size)) header.del = 0;
        } else {
            printf("No Action Bit Set\n");
            free(key);
        }
    } else {
        header.get = 0;
        header.del = 0;
        free(key);
    }

    // CASE 1a NACHRICHT VON ANDEREM PEER
    if (header.intern) {
        open_request_response_peer(hash_data, header, buf);
    } else {
    // CASE 1b NACHRICHT VON CLIENT
        open_request_response_direct(hash_data, socketfd, header, buf);
    }
    return;
}

// ----------------------------------------------------------------------------------------------------------------- //

void send_chord_join(const char* joinIP, const char* joinPort) {
    void *package;
    package_header header;
    int socketfd, nbytes, status;

    socketfd = connect_to(joinIP, joinPort);
    if (socketfd < 0) {
        printf("Chord-join failed while connecting.\n");
        intHandler();
    }

    memset(&header, 0, sizeof(header));
    header.intern = 1;
    header.join = 1;
    header.id = (unsigned short) nodeID;
    header.ip = myip;
    header.port = myport;

    nbytes = marshall_intern(&package, NULL, NULL, 0, header);
    status = send_data(socketfd, package, nbytes);

    if (status < 0) {
        printf("Chord-join failed while sending.\n");
        free(package);
        intHandler();
    }

    close(socketfd);
    free(package);
    return;
}

void response_join(package_header header, void* buf) {
    void *package;
    struct sockaddr_in sa;
    int newID, socketfd, status, nbytes;

    newID = (int) header.id;

    if ((newID <= nodeID && newID > preID) || (preID > nodeID && (newID <= nodeID || newID > preID))
        || preID == nodeID) {
        // Vorgaenger setzen
        preID = newID;
        snprintf(prePort, 7, "%d", ntohs(header.port));
        sa.sin_addr.s_addr = header.ip;
        inet_ntop(AF_INET, &(sa.sin_addr), preIP, INET_ADDRSTRLEN);

        // Notify verschicken
        memset(&header, 0, sizeof(header));
        header.intern = 1;
        header.notify = 1;
        header.id = (unsigned short) nodeID;
        header.ip = myip;
        header.port = myport;

        socketfd = connect_to(preIP, prePort);
        if (socketfd < 0) {
            printf("Can't connect to joining peer.\n");
            free(buf);
            intHandler();
        }
        nbytes = marshall_intern(&package, NULL, NULL, 0, header);
        status = send_data(socketfd, package, nbytes);
        close(socketfd);
        if (status < 0) {
            printf("Chord-join failed while sending.\n");
            free(buf);
            free(package);
            intHandler();
        }
        free(package);
    } else {
        // Join weiterleiten
        socketfd = connect_to(sucIP, sucPort);
        if (socketfd < 0) {
            printf("Can't connect to successor.\n");
            free(buf);
            intHandler();
        }
        status = send_data(socketfd, buf, 14);
        if (status == -1) printf("Can't forward join.\n");
        close(socketfd);
    }
    free(buf);
    return;
}

void response_notify(package_header header) {
    void *package;
    struct sockaddr_in sa;
    int socketfd, status, nbytes;

    // nachfolger setzen
    sucID = (int) header.id;
    snprintf(sucPort, 7, "%d", ntohs(header.port));
    sa.sin_addr.s_addr = header.ip;
    inet_ntop(AF_INET, &(sa.sin_addr), sucIP, INET_ADDRSTRLEN);

    // stabilize an nachfolger senden
    memset(&header, 0, sizeof(header));
    header.intern = 1;
    header.stabilize = 1;
    header.id = (unsigned short) nodeID;
    header.ip = myip;
    header.port = myport;

    socketfd = connect_to(sucIP, sucPort);
    if (socketfd < 0) {
        printf("Can't connect to next peer.\n");
        intHandler();
    }
    nbytes = marshall_intern(&package, NULL, NULL, 0, header);
    status = send_data(socketfd, package, nbytes);
    if (status < 0) printf("Can't send stabilize.\n");
    close(socketfd);
    free(package);
    return;
}

void response_stabilize(package_header header, void* buf) {
    uint32_t ip4;
    void *package;
    char portstr[8];
    unsigned short port;
    struct sockaddr_in sa;
    char ip4str[INET_ADDRSTRLEN + 1];                   // space to hold the IPv4 string
    int socketfd, newID, status, nbytes;

    newID = (int) header.id;
    if (preID < 0) {
        // vorgänger setzen
        preID = newID;
        snprintf(prePort, 7, "%d", ntohs(header.port));
        sa.sin_addr.s_addr = header.ip;
        inet_ntop(AF_INET, &(sa.sin_addr), preIP, INET_ADDRSTRLEN);
    } else if (newID != preID) {
        snprintf(portstr, 7, "%d", ntohs(header.port));
        sa.sin_addr.s_addr = header.ip;
        inet_ntop(AF_INET, &(sa.sin_addr), ip4str, INET_ADDRSTRLEN);

        translate_ip_and_port(preIP, prePort, &ip4, &port);
        if (ip4 == 0 && port == 0) {
            printf("Can't convert IP and Port.\n");
            free(buf);
            intHandler();
        }

        // Notify verschicken
        memset(&header, 0, sizeof(header));
        header.intern = 1;
        header.notify = 1;
        header.id = (unsigned short) preID;
        header.ip = ip4;
        header.port = port;

        socketfd = connect_to(ip4str, portstr);
        if (socketfd < 0) {
            printf("Can't connect to other peer to send notify.\n");
            free(buf);
            intHandler();
        }

        nbytes = marshall_intern(&package, NULL, NULL, 0, header);
        status = send_data(socketfd, package, nbytes);
        if (status < 0) printf("Stabilizing failed while sending.\n");

        close(socketfd);
        free(package);
    } else {
        // stabilize "weiterleiten"
        memset(&header, 0, sizeof(header));
        header.intern = 1;
        header.stabilize = 1;
        header.id = (unsigned short) nodeID;
        header.ip = myip;
        header.port = myport;

        socketfd = connect_to(sucIP, sucPort);
        if (socketfd < 0) {
            printf("Can't connect to successor.\n");
            free(buf);
            intHandler();
        }

        nbytes = marshall_intern(&package, NULL, NULL, 0, header);
        status = send_data(socketfd, package, nbytes);
        if (status == -1) printf("Can't forward stabilize.\n");
        close(socketfd);
        free(package);
    }
    free(buf);
    return;
}

// ----------------------------------------------------------------------------------------------------------------- //

void read_package(int socketfd, void* buf) {
    void *key;
    unsigned int hash;
    package_header header;

    // we got some data from a client
    header = unmarshall(buf);
    protokoll_debug(buf, header, preID, sucID, preIP, prePort, sucIP, sucPort);

    // CASE 0 NACHRICHT VON PEER
    if (header.intern) {
        if (header.ack) {
            completed_request(header, buf);
            return;
        } else if (header.join) {
            response_join(header, buf);
            return;
        } else if (header.notify) {
            free(buf);
            response_notify(header);
            return;
        } else if (header.stabilize) {
            response_stabilize(header, buf);
            return;
        }
    }

    if (preID < 0) {
        printf("Chord-join failed!?\n");
        free(buf);
        intHandler();
    }

    key = (char *) buf + 6 + (8 * header.intern);
    hash = calc_hash(key, header.key_size, HASHSIZE);

    // CASE 1 PEER IST ZUSTÄNDIG
    if (((int) hash <= nodeID && (int) hash > preID) || (preID > nodeID && ((int) hash <= nodeID
        || (int) hash > preID)) || preID == nodeID) {
        open_request_response(socketfd, header, buf);
    } else {
    // CASE 2 PEER NICHT ZUSTÄNDIG
        open_request_forward(socketfd, header, buf);
    }
    free(buf);
    return;
}

void check_sockets() {
    void *buf;
    int nbytes, newfd;                        // socket file descriptors
    struct sockaddr_storage remoteaddr;                 // adressen vom remote
    socklen_t addrlen = sizeof(remoteaddr);             // groessen von adressen von remote

    // main loop
    while(1) {
        read_fds = master; // copy it
        if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
            printf("can't call select\n");
            exit(1);
        }

        // run through the existing connections looking for data to read
        for (int i = 0; i <= fdmax; i++) {
            if (FD_ISSET(i, &read_fds)) { // we got one!!
                if (i == listener) {
                    newfd = accept(listener, (struct sockaddr *) &remoteaddr, &addrlen);
                    if (newfd == -1) printf("Failed accepting new connection.\n");
                    else {
                        FD_SET(newfd, &master); // add to master set
                        if (newfd > fdmax) fdmax = newfd;
                    }
                } else {
                    if ((nbytes = receive_data(i, &buf)) <= 0) {
                        // got error or connection closed by client
                        if (nbytes == -1) {
                            printf("Error while receiving data.\n");
                        }
                        remove_socket_list(&transid_list, i);
                        close(i); // bye!
                        FD_CLR(i, &master); // remove from master set
                    } else {
                        read_package(i, buf);
                    } // END if connection closed or received package
                } // END if new_connection or data from client
            } // END if file descriptor
        } // END looping up to max file descriptor
    } // END main loop
    return;
}

void check_input(const int argc, const char* argv[]) {
    // Kontrolliere ob Eingabeparameter vorhanden sind
    if (!(argc == 3 || argc == 4 || argc == 6)) {
        printf("Das Programm sollte wie folgt aufgerufen werden:\n");
        printf("./peer IP Port [ID] [Peer-IP Peer-Port]\n");
        exit(1);
    }

    // Kontrolliere ob Ports in Ordnung sind
    long input;
    input = atol(argv[2]);
    if (input < 0 || input > 65535) {
        printf("Bitte Ports zwischen 0 und 65535 angeben.\n");
        exit(1);
    }

    if (argc == 6) {
        input = atol(argv[5]);
        if (input < 0 || input > 65535) {
            printf("Bitte Ports zwischen 0 und 65535 angeben.\n");
            exit(1);
        }
    }

    if (argc > 3) {
        input = atol(argv[3]);
        if (input < 0 || input > HASHSIZE - 1) {
            printf("Bitte IDs zwischen 0 und %d angeben.\n", HASHSIZE - 1);
            exit(1);
        }
        nodeID = atoi(argv[3]);
    } else nodeID = 0;

    if (HASHSIZE > 65536) {
        printf("Max HASHSIZE is 65536. Please recompile.\n");
        exit(1);
    }
}

int main(const int argc, const char* argv[]) {
    check_input(argc, argv);

    FD_ZERO(&master);                                   // clear the master and temp sets
    FD_ZERO(&read_fds);

    listener = listen_to(argv[1], argv[2], &myip, &myport);
    FD_SET(listener, &master);
    fdmax = listener;

    hashmap.size = 0;
    transid_list = init_list();
    signal(SIGINT, intHandler);

    if (argc <= 4) {
        strncpy(preIP, argv[1], INET_ADDRSTRLEN);
        strncpy(sucIP, argv[1], INET_ADDRSTRLEN);
        strncpy(prePort, argv[2], 8);
        strncpy(sucPort, argv[2], 8);
        preID = nodeID;
        sucID = nodeID;
    } else {
        strncpy(preIP, "", 1);
        strncpy(sucIP, "", 1);
        strncpy(prePort, "", 1);
        strncpy(sucPort, "", 1);
        preID = -1;
        sucID = -1;
        send_chord_join(argv[4], argv[5]);
    }

    check_sockets();
    return 0;
}

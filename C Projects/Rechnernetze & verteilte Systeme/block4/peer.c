/*
    Rechnernetze und Verteilte Systeme - Block 4; Aufgabe 3;
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
#include "hashmap.h"
#include "protokoll.h"
#include "connection.h"

#ifndef HASHSIZE
#define HASHSIZE 512
#endif

// ----------------------------------------------------------------------------------------------------------------- //

int fdmax;                              // track biggest fd
qhead transid_list;
fd_set master, read_fds;
hash_map hashmap;

// ----------------------------------------------------------------------------------------------------------------- //

void intHandler() {
    for (int i = 0; i <= fdmax; i++) {
        if (FD_ISSET(i, &master)) close(i);
    }
    free_list(&transid_list);
    free_hashmap(hashmap);
    exit(0);
}

// ----------------------------------------------------------------------------------------------------------------- //

void checkArguments(const int argc, const char* argv[]){
    if (argc != 10) {
        printf("The program should be called as followed:\n\t./program ID IP PORT preID preIP prePORT sucID sucIP sucPORT\n");
        exit(1);
    }
    long port,id;
    for(int i=0;i<3;i++){
        id = atol(argv[1 + 3*i]);
        if(id < 0 || id > 65535) {
            fprintf(stderr,"Wrong ID at argument %d !! please choose an ID between 1 and 65535.\n",(1+i*3));
            exit(1);
        }
        port = atol(argv[3 + i*3]);
        if(port < 0 || port > 65535) {
            fprintf(stderr,"Wrong port at argument %d !! please choose a port between 1 and 65535.\n",(3+i*3));
            exit(1);
        }
    }
}

int main(const int argc, const char* argv[]) {
    checkArguments(argc,argv);
    signal(SIGINT, intHandler);

    uint32_t myip;
    uint8_t transFinder;
    unsigned int hash;
    int *rv, status;
    int nbytes, listener, newfd;                        // socket file descriptors
    unsigned short nodeID, preID, hashrange, myport;
    void *buf, *key, *value, *package;
    char portstr[8];
    char ip4str[INET_ADDRSTRLEN];                       // space to hold the IPv4 string
    struct sockaddr_in sa;                              // pretend this is loaded with something
    struct sockaddr_storage remoteaddr;                 // adressen vom remote
    socklen_t addrlen = sizeof(remoteaddr);             // groessen von adressen von remote
    hash_entry *hash_data = NULL;
    package_header header;
    transid_list = init_list();

    listener = listen_to(argv[2], argv[3], &myip, &myport);

    FD_ZERO(&master);                                   // clear the master and temp sets
    FD_ZERO(&read_fds);
    FD_SET(listener, &master);
    fdmax = listener;

    nodeID = atoi(argv[1]);
    preID = atoi(argv[4]);
    hashrange = (nodeID < preID) ? nodeID + HASHSIZE - preID - 1 : nodeID - preID;

    hashmap = create_hashmap(hashrange);

    
    while(1) {
        read_fds = master;
        if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
            printf("can't call select\n");
            exit(1);
        }

        // keeps looping between connections listening for requests
        for (int i = 0; i <= fdmax; i++) {
            if (FD_ISSET(i, &read_fds)) {
                if (i == listener) {
                    if((newfd = accept(listener, (struct sockaddr *) &remoteaddr, &addrlen)) == -1)
                        printf("Failed accepting new connection.\n");
                    else {
                        FD_SET(newfd, &master);
                        if (newfd > fdmax) fdmax = newfd;
                    }
                } else {
                    if ((nbytes = receive_data(i, &buf)) <= 0) {
                        // got error or connection closed by client
                        if (nbytes == -1)
                            printf("Error while receiving data.\n");
                        // remove_socket_list(&transid_list, i);
                        close(i);
                        FD_CLR(i, &master); // remove from master set
                    } else {
                        header = unmarshall(buf);
                        protokoll_debug(buf, header, argv);
// CASE 0 REPLY FROM PEER
                        if (header.intern && header.ack) {
                            printf("antwort von peer\n");
                            // rv = find_list(&transid_list, header.transid);
                            // if (!rv) {
                            //     printf("Client disconnected? Can't find open Transaction.\n");
                            //     free(buf);
                            //     continue;
                            // }
                            // newfd = rv[0];
                            // // header.transid = rv[1];
                            // free(rv);
                            // remove_transid_list(&transid_list, header.transid);

                            nbytes = marshall_answer(&package, buf, (void *) ((char *) buf + 17 + header.key_size),
                                header.value_size, header);
                            printf("package size before sending %d header size %d\n",sizeof(package),header.value_size);
                            status = send_data(newfd, package, nbytes);
                            if (status == -1) printf("Can't answer client.\n");
                            if (status != nbytes) printf("hasn't sent everything.\n");
                            free(package);
                            free(buf);
                            continue;
                        }
                        key = (char *) buf + 7 + (8 * header.intern);
                        hash = calc_hash(key, header.key_size, HASHSIZE);


// CASE 1 PEER IS RESPONSIBLE
                        if ((hash <= nodeID && hash > preID) || (preID > nodeID && (hash <= nodeID || hash > preID))) {
                            printf("peer responsible\n");
                            key = calloc(header.key_size,1);
                            memcpy(key, (void *) ((char *) buf + 7), header.key_size);

                            if (header.get) {
                                if(!(hash_data = get(hashmap, key, header.key_size))) header.get = 0;
                            } else if (header.set) {
                                value = calloc(header.value_size,1);
                                memcpy(value, (void *) ((char *) buf + 7 + 10*header.intern + header.key_size),header.value_size);
                                set(hashmap, key, header.key_size, value, header.value_size);
                            } else if (header.del) {
                                if(!del(hashmap, key, header.key_size)) header.del = 0;
                            } else {
                                printf("No Action Bit Set\n");
                                free(key);
                                free(buf);
                                continue;
                            }

// CASE 1a MSG FROM OTHER PEER
                            if (header.intern) {
                                printf("peer responsible !! internal msg\n");
                                snprintf(portstr, 7, "%d", ntohs(header.port));
                                sa.sin_addr.s_addr = header.ip;
                                inet_ntop(AF_INET, &(sa.sin_addr), ip4str, INET_ADDRSTRLEN);

                                header.ack = 1;
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
                                if (status != nbytes) printf("Not sended everything.\n");
                                close(newfd);
                            } else {
// CASE 1b NACHRICHT VON CLIENT
                                printf("msg from client\n");
                                if (hash_data){ nbytes = marshall_answer(&package, buf, hash_data->value,
                                    hash_data->value_size, header);
                                    // printf("start marshall with v_size %d\n",hash_data->value_size);
                                }
                                else nbytes = marshall_answer(&package, buf, NULL, 0, header);
                                status = send_data(i, package, nbytes);
                                if (status == -1) printf("Can't answer Client.\n");
                                if (status != nbytes) printf("Not sended everything.\n");
                            }
                            close(i);
                            FD_CLR(i, &master); // remove from master set
                            free(package);
                            hash_data = NULL;
                        } else {
// CASE 2 PEER NOT RESPONSIBLE
                            printf("not responsible\n");
                            newfd = connect_to(argv[8], argv[9]);
                            if (newfd == -1) {
                                printf("Can't connect to next peer.\n");
                                free(buf);
                                intHandler();
                            }
// CASE 2a NACHRICHT VON ANDEREM PEER
                            if (header.intern) {
                                printf("msg from other peer\n");
                                status = send_data(newfd, buf, nbytes);
                                if (status == -1) printf("Can't forward to Peer.\n");
                                if (status != nbytes) printf("Not sended everything.\n");
                            }
// CASE 2b NACHRICHT VON CLIENT
                            else {
                                printf("msg from client !!\n");
                                header.id = nodeID;
                                header.ip = myip;
                                header.port = myport;
                                header.hashID = hash;
                                nbytes = marshall_intern(&package, buf,
                                    (void *) ((char *) buf + 7 + header.key_size), header.value_size, header);
                                status = send_data(newfd, package, nbytes);
                                if (status == -1) printf("Can't forward to Peer.\n");
                                if (status != nbytes) printf("Not sended everything.\n");
                                free(package);
                            }
                            close(newfd);
                        }
                        free(buf);
                    } // END handle receive
                } // END handle data from client
            } // END got new incoming connection
        } // END looping through file descriptors
    } // END main loop

    return 0;
}

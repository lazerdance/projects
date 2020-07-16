#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>

#include "protokoll.h"
#include "hashmap.h"
#include "connection.h"

#ifndef HASHSIZE
#define HASHSIZE 512
#endif


int fdmax;
fd_set master, read_fds;
hash_map hashmap;


uint32_t myip,sucIP;
uint16_t transFinder;
int *rv, status;
int nbytes, listener, newfd,safe_nbytes=0,safe_fd=0;                        // socket file descriptors
unsigned short nodeID, preID, sucID, hashrange, myport,sucPORT;
void *buf, *key, *value, *package,*safe_package;
char portstr[8];
char ip4str[INET_ADDRSTRLEN];                       // space to hold the IPv4 string
struct sockaddr_in sa;
struct sockaddr_storage remoteaddr;                 // storage for remote addresses
socklen_t addrlen = sizeof(remoteaddr);             // size of remote addresses storage
hash_entry *hash_data = NULL;
package_header header;

unsigned safe_hash;
void intHandler() {
    for (int i = 0; i <= fdmax; i++) {
        if (FD_ISSET(i, &master)) close(i);
    }
    free_hashmap(hashmap);
    exit(0);
}

void input_check(const int argc, const char* argv[]){
    if (argc != 10) {
        printf("Das Programm sollte wie folgt aufgerufen werden:\n");
        printf("./peer myID myIP myPORT preID preIP prePORT sucID sucIP sucPORT\n");
        exit(1);
    }

    long input;
    for (int i = 0; i < 3; ++i) {
        input = atol(argv[3 + 3 * i]);
        if (input < 0 || input > 65535) {
            printf("Bitte Ports zwischen 0 und 65535 angeben.\n");
            exit(1);
        }
        input = atol(argv[1 + 3 * i]);
        if (input < 1 || input > 65535) {
            printf("Bitte IDs zwischen 0 und 65535 angeben.\n");
            exit(1);
        }
    }

}

unsigned int ip_to_int (const char * ip)
{
    unsigned v = 0;
    int i;
    const char * start;
    start = ip;
    for (i = 0; i < 4; i++) {
        char c;
        int n = 0;
        while (1) {
            c = * start;
            start++;
            if (c >= '0' && c <= '9') {
                n *= 10;
                n += c - '0';
            }
            else if ((i < 3 && c == '.') || i == 3)
                break;
            else
                return 0;
        }
        if (n >= 256)
            return 0;
        v *= 256;
        v += n;
    }
    return v;
}


void terminate_connection(int fd){
    close(fd);
    FD_CLR(fd, &master);
    fdmax--;
}

int main(const int argc, const char* argv[]) {

    input_check(argc,argv);

    signal(SIGINT, intHandler);
    listener = listen_to(argv[2], argv[3], &myip, &myport);


    sucIP = htonl(ip_to_int(argv[2]));
    sucPORT = atoi(argv[9]);

    FD_ZERO(&master);                                   // clear the master and temp sets
    FD_ZERO(&read_fds);
    FD_SET(listener, &master);
    fdmax = listener;

    nodeID = atoi(argv[1]);
    preID = atoi(argv[4]);
    sucID = atoi(argv[7]);

    hashrange = nodeID - preID;
    if (nodeID < preID) {
        hashrange = nodeID + HASHSIZE - preID - 1;
    }

    hashmap = create_hashmap(hashrange);

    while(1) {
        printf("looping for connection...\n...\n...\n");
        read_fds = master;
        if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
            printf("can't call select\n");
            exit(1);
        }

        // run through the existing connections looking for data to read
        for (int i = 0; i <= fdmax; i++) {
            if (FD_ISSET(i, &read_fds)) {           // if a connection is established
                printf("fdset %d and listener %d\n",i,listener);
                if (i == listener) {
                    newfd = accept(listener, (struct sockaddr *) &remoteaddr, &addrlen);
                    printf("\n## connection received from %d\n",newfd);
                    if (newfd == -1) printf("Failed accepting new connection.\n");
                    else {
                        printf("fdset newfd %d\n",newfd);
                        FD_SET(newfd, &master);     // add to master set
                        if (newfd > fdmax) fdmax = newfd;
                    }
                } else {
                    printf("receiving data \n");
                    if ((nbytes = receive_data(newfd, &buf)) <= 0) {
                    printf("received data \n");
                        // got error or connection closed by client
                        if (nbytes == -1){
                            printf("Error while receiving data.\n");
                            terminate_connection(i);
                        }
                    } else {
                        header = unmarshall(buf);
                        protokoll_debug(buf, header, argv);
                        printf("\n################## entering cases ############\n");
                        printf("\n##############################################\n\n");
// CASE 1 : Internal Message
                        if (header.intern) {
                            printf("CASE 1 : INTERN\n");
                            if(header.reply){
                                terminate_connection(newfd);
                                printf("CASE 1a : header.reply\n");
                                snprintf(portstr, 7, "%d", header.port);
                                sa.sin_addr.s_addr = ntohl(header.ip);
                                inet_ntop(AF_INET, &(sa.sin_addr), ip4str, INET_ADDRSTRLEN);

                                delete_elem(header.hashID,newfd);
                                printf("connecting to %s with port %s\n",ip4str,portstr);                                
                                newfd = connect_to(ip4str, portstr);
                                if (newfd == -1) {
                                    printf("Can't connect to a peer.\n"); free(safe_package); intHandler();
                                }
    
                                new_elem(header.hashID,newfd);
                                safe_hash = header.hashID;
                                if(!FD_ISSET(newfd, &master)){
                                    printf("%d isnt set yet\n",newfd);
                                    FD_SET(newfd,&master);
                                    if(newfd>fdmax) fdmax = newfd+1;
                                }
                                printf("sending safe_package with nbytes %d\n",nbytes);
                                status = send_data(newfd, safe_package, safe_nbytes);
                                printf("data sent to %d\n",newfd);
                                free(safe_package);
                                if (status == -1) printf("Can't answer client.\n");
                                if (status != nbytes) printf("Not sended everything.\n");
                            }else if(header.lookup){
                                printf("CASE 1b : header.lookup\n");
                                if ((header.hashID <= sucID && header.hashID > nodeID) || (nodeID > sucID && (header.hashID<= sucID || header.hashID > nodeID)))
                                {
                                    printf("CASE 1b_1 : Nachfolger zuständig\n");
                                    header.reply = 1;
                                    header.lookup = 0;
                                    printf("terminating %d\n",newfd);
                                    struct sockaddr_in sin;
                                    socklen_t len = sizeof(sin);
                                    if (getsockname(newfd, (struct sockaddr *)&sin, &len) == -1)
                                        perror("getsockname");
                                    else
                                        printf("port number %d\n", ntohs(sin.sin_port));
                                    if(ntohs(sin.sin_port) !=  atoi(argv[3]))
                                        terminate_connection(newfd);
                                    snprintf(portstr, 7, "%d", header.port);
                                    sa.sin_addr.s_addr = header.ip;
                                    inet_ntop(AF_INET, &(sa.sin_addr), ip4str, INET_ADDRSTRLEN);
                                    
                                    printf("connecting to %s with port %s\n",ip4str,portstr);
                                    newfd = connect_to(ip4str, portstr);
                                    // pack sucInfo in the reply header
                                    header.id = sucID;
                                    header.ip = htonl(sucIP);
                                    header.port = htons(sucPORT);
                                    printf("marshalling internal request\n");
                                    nbytes = marshall_internal_request(&package, header);
                                    printf("sending data\n");
                                    status = send_data(newfd, package, nbytes);
                                    if (status == -1) printf("Can't answer client.\n");
                                    if (status != nbytes) printf("Not sended everything.\n");
                                    free(package);
                                    printf("terminating %d\n",newfd);
                                    terminate_connection(newfd);
                                }else{
                                    printf("CASE 1b_2 : Nachfolger nicht zuständig\n");
                                    newfd = connect_to(argv[8], argv[9]);
                                    if (newfd == -1) {
                                        printf("Can't connect to next peer.\n");
                                        free(buf); intHandler();
                                    }
                                    // FD_CLR(newfd,&master);
                                    status = send_data(newfd, buf, nbytes);
                                    if (status == -1) printf("Can't forward to Peer.\n");
                                    if (status != nbytes) printf("Not sended everything.\n");
                                    printf("terminating %d\n",newfd);
                                    terminate_connection(newfd);
                                }
                            }
                        }else{
// CASE 2 EXTERN        
                            printf("CASE 2 EXTERN\n");
                            key = (char *) buf + 7;
                            header.hashID = calc_hash(key, header.key_size, HASHSIZE);
                            printf("calculated hash %d\n",header.hashID);
// CASE 2a PEER IST ZUSTÄNDIG
                            if(header.ack){
                                printf("CASE 2a : Antwort von Peer.. an Client weiterleiten\n");
                                if(safe_fd){
                                    printf("sending data to safe_fd %d with safe_hash %d and nbytes %d\n",safe_fd,safe_hash,nbytes);
                                    status = send_data(safe_fd, buf, nbytes);
                                }else{
                                    printf("sending data to newfd %d with safe_hash %d and nbytes %d\n",newfd,safe_hash,nbytes);
                                    newfd = get_sockfd(safe_hash);
                                    status = send_data(newfd, buf, nbytes);
                                }
                                if (status == -1) printf("Can't answer Client.\n");
                                if (status != nbytes) printf("Not sended everything.\n");
                                printf("terminating %d\n",newfd);
                                terminate_connection(newfd);
                                if(safe_fd)
                                    terminate_connection(safe_fd);
                                delete_elem(safe_hash,newfd);
                                delete_elem(safe_hash,safe_fd);
                                fdmax--;
                                safe_hash = 0; safe_fd = 0;
                            }
                            else if ((header.hashID<= nodeID && header.hashID> preID) || (preID > nodeID && (header.hashID<= nodeID || header.hashID> preID)))
                            {
                                printf("CASE 2b : PEER IST ZUSTÄNDIG.. Nachricht von Client\n");
                                key = calloc(header.key_size,1);
                                memcpy(key, (void *) ((char *) buf + 7 + (8 * header.intern)), header.key_size);
                                if (header.get) {
                                    if(!(hash_data = get(hashmap, key, header.key_size))) header.get = 0;
                                } else if (header.set) {
                                    value = calloc(header.value_size,1);
                                    memcpy(value, (void *) ((char *) buf + 7 + (8 * header.intern) + header.key_size),
                                        header.value_size);
                                    set(hashmap, key, header.key_size, value, header.value_size);
                                } else if (header.del) {
                                    if(!del(hashmap, key, header.key_size)) header.del = 0;
                                } else {
                                    printf("No Action Bit Set\n");
                                    free(key); free(buf);continue;
                                }
                                header.ack = 1;
                                if (hash_data) nbytes = marshall_answer(&package, buf, hash_data->value,
                                    hash_data->value_size, header);
                                else nbytes = marshall_answer(&package, buf, NULL, 0, header);
                                protokoll_debug(package,header,argv);
                                printf("sending data to %d\n",newfd);
                                status = send_data(newfd, package, nbytes);
                                printf("sent data to %d\n",newfd);
                                if (status == -1) printf("Can't answer Client.\n");
                                if (status != nbytes) printf("Not sended everything.\n");
                                free(package);
                                hash_data = NULL;
                                printf("terminating %d\n",newfd);
                                terminate_connection(newfd);
                            }
// CASE 2b Nachfolger IST ZUSTÄNDIG
                            else if ((header.hashID <= sucID && header.hashID > nodeID) || (nodeID > sucID && (header.hashID <= sucID || header.hashID > nodeID)))
                            {   
                                printf("CASE 2c : NACHFOLGER IST ZUSTÄNDIG\n");
                                new_elem(header.hashID,newfd);
                                safe_hash = header.hashID;
                                
                                newfd = connect_to(argv[8], argv[9]);
                                if (newfd == -1) {
                                    printf("Can't connect to next peer.\n");
                                    free(buf); intHandler();
                                }
                                if(!FD_ISSET(newfd, &master)){
                                    FD_SET(newfd,&master);
                                    if(newfd>fdmax) fdmax = newfd;
                                }

                                printf("\n## sending data to %d\n",newfd);
                                status = send_data(newfd, buf, nbytes);
                                printf("\n## data sent to %d\n",newfd);
                                if (status == -1) printf("Can't forward to Peer.\n");
                                if (status != nbytes) printf("Not sended everything.\n");
                            }else {
                                printf("CASE 2d : Peer sowie Nachfolger nicht zuständig -> Internal Nachricht\n");
                                new_elem(header.hashID,newfd);
                                safe_hash = header.hashID;
                                safe_fd = newfd;
                                header.lookup = 1;
                                header.id = nodeID;
                                header.ip = myip;
                                header.port = myport;
                                safe_package = calloc(7+header.key_size+header.value_size,1);
                                memcpy(safe_package,buf,7+header.key_size+header.value_size);
                                safe_nbytes = 7 + header.key_size + header.value_size;
                                nbytes = marshall_internal_request(&package, header);
                                printf("connecting to successor\n");
                                newfd = connect_to(argv[8], argv[9]);
                                if (newfd == -1) {
                                    printf("Can't connect to next peer.\n");
                                    free(buf); intHandler();
                                }
                                if(!FD_ISSET(newfd, &master)){
                                    FD_SET(newfd,&master);
                                    if(newfd>fdmax) fdmax = newfd;
                                }
                                status = send_data(newfd, package, nbytes);
                                if (status == -1) printf("Can't forward to Peer.\n");
                                if (status != nbytes) printf("Not sended everything.\n");
                                free(package);
                                printf("terminating %d\n",newfd);
                                terminate_connection(newfd);
                            }
                        }
                        free(buf);
                    } // END handle receive
                } // END handle data from client
            } // END got new incoming connection
        } // END looping through file descriptors
    } // END main loop
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>
#include <time.h>
#include <errno.h>

#include "protokoll.h"
#include "hashmap.h"
#include "connection.h"
#include "fingertable.h"

#ifndef HASHSIZE
#define HASHSIZE 65535
#endif
#ifndef FINGERSIZE
#define FINGERSIZE 16
#endif


int fdmax;
fd_set master, read_fds;
hash_map hashmap;
finger fingertable[FINGERSIZE];
struct timeval tv;
uint32_t myIP,sucIP,preIP;
uint16_t transFinder;
int *rv, status;
int nbytes, listener, newfd,safe_nbytes=0,safe_fd=0,input=0,has_finger_table=0,only_fingertable_lookups=0,ft_counter=0;
unsigned short myID, hashrange, myport,sucPort,prePort;
short preID, sucID;
char prePort_str[8], sucPort_str[8], preIP_str[INET_ADDRSTRLEN + 1], sucIP_str[INET_ADDRSTRLEN + 1];
void *buf, *key, *value, *package,*safe_package;
char portstr[8];
char ip4str[INET_ADDRSTRLEN];                       // space to hold the IPv4 string
struct sockaddr_in sa;
struct sockaddr_storage remoteaddr;                 // storage for remote addresses
socklen_t addrlen = sizeof(remoteaddr);             // size of remote addresses storage
hash_entry *hash_data = NULL;
package_header header;

void red () {
  printf("\033[1;31m");
}

void cyan () {
  printf("\033[0;36m");
}

void yellow (){
  printf("\033[1;33m");
}

void magenta (){
  printf("\033[0;35m");
}

void reset () {
  printf("\033[0m");
}

void printHashRange(){
    magenta();
    printf("hashrange is now \033[1;33m %d\n",hashrange);
    reset();
}

unsigned safe_hash;
void intHandler() {
    for (int i = 0; i <= fdmax; i++) {
        if (FD_ISSET(i, &master)) close(i);
    }
    free_hashmap(hashmap);
    exit(0);
}

void input_check(const int argc, const char* argv[]){
    if (!(argc == 3 || argc == 4 || argc == 6)) {
        printf("Das Programm sollte wie folgt aufgerufen werden:\n");
        printf("./peer IP Port [ID] [Peer-IP Peer-Port]\n");
        exit(1);
    }

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
        myID = atoi(argv[3]);
    } else myID = 0;

    if (HASHSIZE > 65536) {
        printf("Max HASHSIZE is 65536. Please recompile.\n");
        exit(1);
    }
}

void terminate_connection(int fd){
    close(fd);
    FD_CLR(fd, &master);
    fdmax--;
}


void setHashRange(){
    if (myID < preID)
        hashrange = myID + HASHSIZE - preID - 1;
    else if(myID == preID)
        hashrange = HASHSIZE;
    else hashrange = myID - preID;
}

void set_header_cmd(int intern,int f_ack, int join, int notify, int stabilize, int reply, int lookup){
    // memset(&header, 0, sizeof(header));
    header.intern = intern;
    header.f_ack = f_ack;
    header.join = join;
    header.notify = notify;
    header.stabilize = stabilize;
    header.reply = reply;
    header.lookup = lookup;
}

package_header create_header_join(){
    set_header_cmd(1,0,1,0,0,0,0);
    header.id = myID;
    header.ip = myIP;
    header.port = myport;
    return header;
}

package_header create_header_stabilize(){
    set_header_cmd(1,0,0,0,1,0,0);
    header.id = myID;
    header.ip = myIP;
    header.port = myport;
    return header;
}


package_header create_header_notify(){
    set_header_cmd(1,0,0,1,0,0,0);
    header.id = (unsigned short) preID;
    header.ip = preIP;
    header.port = prePort;
    return header;
}

void connect_and_send(int save_contact, int free_pkg){
    newfd = connect_to(sucIP_str, sucPort_str);
    if (newfd == -1) {
        printf("Can't connect to next peer.\n");
        free(buf); intHandler();
    }
    if(save_contact && !FD_ISSET(newfd, &master)){
        FD_SET(newfd,&master);
        if(newfd>fdmax) fdmax = newfd;
    }
    status = send_data(newfd, package, nbytes);
    if (status == -1) printf("Can't forward to Peer.\n");
    if (status != nbytes) printf("Not everything sent.\n");
    if (free_pkg) free(package);
    terminate_connection(newfd);
}

//--------------------------------------------------------------------------------------
void response_join(package_header header, void* buf) {
    printf("CASE 1 : join\n");
    void *package;
    struct sockaddr_in sa;
    int socketfd, status, nbytes;

    if ((header.id <= myID && header.id > preID) || (preID > myID 
    && (header.id <= myID || header.id > preID)) || preID == myID) {
        // set Predecessor
        preID = header.id;
        snprintf(prePort_str, 7, "%u", header.port);
        sa.sin_addr.s_addr = header.ip;
        inet_ntop(AF_INET, &(sa.sin_addr), preIP_str, INET_ADDRSTRLEN);
        if(sucID == myID){
            sucID = header.id;
            snprintf(prePort_str, 7, "%u", header.port);
            inet_ntop(AF_INET, &(sa.sin_addr), sucIP_str, INET_ADDRSTRLEN);
        }
        // Notify verschicken
        memset(&header, 0, sizeof(header));
        header.intern = 1;
        header.notify = 1;
        header.id = (unsigned short) myID;
        header.ip = myIP;
        header.port = myport;
        setHashRange();
        printHashRange();
        hashmap = create_hashmap(hashrange);

        socketfd = connect_to(preIP_str, prePort_str);
        if (socketfd < 0) {
            printf("Can't connect to joining peer.\n");
            free(buf);
            intHandler();
        }
        nbytes = marshall_internal_request(&package, header);
        status = send_data(socketfd, package, nbytes);
        close(socketfd);

        printf("response_join: sent data status %d nbytes %d\n",status,nbytes);
        if (status < 0) {
            printf("Chord-join failed while sending.\n");
            free(buf);
            free(package);
            intHandler();
        }
        free(package);
    } else {
        // Join weiterleiten
        socketfd = connect_to(sucIP_str, sucPort_str);
        if (socketfd < 0) {
            printf("Can't connect to successor.\n");
            free(buf);
            intHandler();
        }
        status = send_data(socketfd, buf, 14);
        if (status == -1) printf("Can't forward join.\n");
        terminate_connection(socketfd);
    }
    return;
}

void response_notify(package_header header) {
    printf("CASE 1 : notify\n");
    void *package;
    struct sockaddr_in sa;
    int socketfd, status, nbytes;

    // set successor
        sucID = header.id;
        snprintf(sucPort_str, 7, "%d", header.port);
        sucPort = header.port;
        sa.sin_addr.s_addr = header.ip;
        sucIP = header.ip;
        inet_ntop(AF_INET, &(sa.sin_addr), sucIP_str, INET_ADDRSTRLEN);
        memset(&header, 0, sizeof(header));
        header.intern = 1;
        header.stabilize = 1;
        header.id = (unsigned short) myID;
        header.ip = myIP;
        header.port = myport;
        setHashRange();
        printHashRange();
        hashmap = create_hashmap(hashrange);
        socketfd = connect_to(sucIP_str, sucPort_str);
        if (socketfd < 0) {
            printf("Can't connect to next peer.\n");
            intHandler();
        }
        nbytes = marshall_internal_request(&package, header);
        status = send_data(socketfd, package, nbytes);
        if (status < 0) printf("Can't send stabilize.\n");
        terminate_connection(socketfd);
        free(package);
        return;
}

void response_stabilize(package_header header, void* buf) {
    void *package;
    struct sockaddr_in sa;
    int socketfd, status, nbytes;
    if (preID < 0) {
        preID = header.id;
        snprintf(prePort_str, 7, "%d", header.port);
        sa.sin_addr.s_addr = header.ip;
        inet_ntop(AF_INET, &(sa.sin_addr), preIP_str, INET_ADDRSTRLEN);
        setHashRange();
        printHashRange();
        hashmap = create_hashmap(hashrange);
    } else if (header.id != preID) {
        snprintf(portstr, 7, "%d", header.port);
        sa.sin_addr.s_addr = header.ip;
        inet_ntop(AF_INET, &(sa.sin_addr), ip4str, INET_ADDRSTRLEN);

        translate_ip_and_port(preIP_str, prePort_str, &preIP, &prePort);
        if (preIP == 0 && prePort == 0) {
            printf("Can't convert IP and Port.\n");free(buf);intHandler();
        }
        socketfd = connect_to(ip4str, portstr);
        if (socketfd < 0) {
            printf("Can't connect to other peer to send notify.\n");
            free(buf);
            intHandler();
        }
        nbytes = marshall_internal_request(&package, create_header_notify());
        setHashRange();
        printHashRange();
        create_hashmap(hashrange);
        status = send_data(socketfd, package, nbytes);
        if (status < 0) printf("Stabilizing failed while sending.\n");
        close(socketfd);
        free(package);
    } else {
        // stabilize "weiterleiten"
        set_header_cmd(1,0,0,0,1,0,0);
        header.id = (unsigned short) myID;
        header.ip = myIP;
        header.port = myport;

        socketfd = connect_to(sucIP_str, sucPort_str);
        header.stabilize = 0;
        if (socketfd < 0) {
            printf("Can't connect to successor.\n");
            free(buf);
            intHandler();
        }

        nbytes = marshall_internal_request(&package, header);
        status = send_data(socketfd, package, nbytes);
        if (status == -1) printf("Can't forward stabilize.\n");
        terminate_connection(socketfd);
        free(package);
    }
    // free(buf);
    return;
}
//----------------------------------------------------------------------


void send_chord_join(const char * known_ip, const char * known_port){
    void *package;
    int socketfd = connect_to(known_ip,known_port);
    if (socketfd < 0) {
        printf("Chord-join failed while connecting.\n");
        intHandler();
    }
    FD_SET(socketfd, &master);
    if (newfd > fdmax) fdmax = newfd;

    nbytes = marshall_internal_request(&package,create_header_join());
    status = send_data(socketfd,package,nbytes);
    if (status == -1) printf("Join request wasnt sent successfully.\n");
    close(socketfd);
}

int sendFingerTableLookup() {
    printf("\n++++++ sendfingertableLookup ++++++ \n");
    void * package;
    if(ft_counter == FINGERSIZE){
        only_fingertable_lookups = 0;
        has_finger_table = 1;
        for (int i = 0; i < FINGERSIZE; i++) {
            printf("finger %d has start %d and ID %d and port %s\n",i,fingertable[i].start,fingertable[i].nodeID,fingertable[i].port);
        }
        memset(&header, 0, sizeof(header));
        header.intern = 1;
        header.f_ack = 1;
        header.finger = 1;
        nbytes = marshall_internal_request(&package, header);
        status = send_data(safe_fd, package, nbytes);
        terminate_connection(safe_fd);
        memset(&header, 0, sizeof(header));
        safe_fd = 0;
        return 0;
    }
    while (ft_counter < FINGERSIZE && ( (fingertable[ft_counter].start <= (unsigned int)sucID && fingertable[ft_counter].start > (unsigned int) myID)
        || (myID > (unsigned short)sucID && (fingertable[ft_counter].start<= (unsigned int) sucID || fingertable[ft_counter].start > (unsigned int) myID) ) ) )
    {
        // printf("sendfingertableLookup: start %d at sucID %d sucID short %d\n",fingertable[ft_counter].start,(unsigned int)sucID, (unsigned short)sucID);
        fingertable[ft_counter].nodeID = sucID;
        strncpy(fingertable[ft_counter].ip,sucIP_str,INET_ADDRSTRLEN);
        strncpy(fingertable[ft_counter].port,sucPort_str,8);
        ft_counter++;
    }
    if(ft_counter < FINGERSIZE){
        memset(&header, 0, sizeof(header));
        header.intern = 1;
        header.lookup = 1;
        header.hashID = fingertable[ft_counter].start;
        header.id = myID;
        header.ip = myIP;
        header.port = myport;
        nbytes = marshall_internal_request(&package, header);
        // printf("sendfingertableLookup: sending fingertable lookup with id %d port %s\n",sucID,sucPort_str);
        newfd = connect_to(sucIP_str, sucPort_str);
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
        terminate_connection(newfd);
    }
    return header.hashID;
}

void send_stabilize(){
    // printf("sucPort %d myport %d\n",sucPort,ntohs(myport));
    if(sucPort == ntohs(myport)) return;
    
    void *package;
    int socketfd;
    // printf("send_stabilize: connecting to port %s myport %d IP %s\n",sucPort_str,ntohs(myport),sucIP_str);
    socketfd = connect_to(sucIP_str,sucPort_str);

    if (socketfd < 0) {
        red();
        printf("send_stabilize failed while connecting.\n");
        reset();
        intHandler();
    }
    if(!FD_ISSET(socketfd,&master)){
        FD_SET(socketfd, &master);
        if (socketfd > fdmax) fdmax = socketfd;
    }

    nbytes = marshall_internal_request(&package,create_header_stabilize());
    status = send_data(socketfd,package,nbytes);
    if (status == -1) printf("send_stabilize request wasnt sent successfully.\n");
    terminate_connection(socketfd);
}

int main(const int argc, const char* argv[]) {
    tv.tv_sec = 2;
    tv.tv_usec = 21000;
    input_check(argc,argv);

    signal(SIGINT, intHandler);
    FD_ZERO(&master);                                   // clear the master and temp sets
    FD_ZERO(&read_fds);

    listener = listen_to(argv[1], argv[2], &myIP, &myport);

    FD_SET(listener, &master);
    fdmax = listener;

    preID = -1;
    sucID = -1;

    if ( argc == 3 || argc == 4) {
        if(argc == 3)   myID = 0;
        else myID = atoi(argv[3]);
        strncpy(preIP_str, argv[1], INET_ADDRSTRLEN);
        strncpy(sucIP_str, argv[1], INET_ADDRSTRLEN);
        strncpy(prePort_str, argv[2], 8);
        strncpy(sucPort_str, argv[2], 8);
        preID = myID;
        sucID = myID;
    } else if(argc == 6){
        strncpy(preIP_str, "", 1);
        strncpy(sucIP_str, "", 1);
        strncpy(prePort_str, "", 1);
        strncpy(sucPort_str, "", 1);
        send_chord_join(argv[4], argv[5]);
    }else intHandler();

    setHashRange();
    printHashRange();
    hashmap = create_hashmap(hashrange);
    time_t curr_time;
    time_t start_time = time(NULL);
    srand(time(0)); 
    while(1) {
        read_fds = master;
        if(tv.tv_sec < 2){
            tv.tv_sec = 2; tv.tv_usec = 20000;
        }
        if (select(fdmax+1, &read_fds, NULL, NULL, &tv) == -1) {
            printf("select error: %s\n",strerror(errno)); exit(1);
        }

        for (int i = 0; i <= fdmax; i++) {
            if (FD_ISSET(i, &read_fds)) {
                if (i == listener) {
                    newfd = accept(listener, (struct sockaddr *) &remoteaddr, &addrlen);
                    if (newfd == -1) printf("Failed accepting new connection.\n");
                    else {
                        FD_SET(newfd, &master);
                        if (newfd > fdmax) fdmax = newfd;
                    }
                } else {
                    if ((nbytes = receive_data(newfd, &buf)) <= 0) {
                        terminate_connection(i);
                    } else {
                        header = unmarshall(buf);
                        protokoll_debug(buf, header);
// CASE 1 : Internal Message
                        if (header.intern) {
                            printf("CASE 1 : INTERN\n");
// CASE 1a : initiate fingertable
                            if (header.finger) {
                                safe_fd = newfd;
                                printf("\n\n\n\t\t\t### header.finger ###\n\n\n\n");
                                init_fingertable(fingertable,myID);
                                only_fingertable_lookups = 1;
                                sendFingerTableLookup();
                            }
// CASE 1b : join request
                            else if(header.join){
                                response_join(header,buf);
                            }
// CASE 1b : notify request
                            else if(header.notify){
                                response_notify(header);
                            }
// CASE 1c : stabilize request
                            else if(header.stabilize){
                                printf("CASE 1 : stabilize\n");
                                response_stabilize(header,buf);
                                if(only_fingertable_lookups && ft_counter > 0 && ft_counter < FINGERSIZE){
                                    printf("header.stabilize: \t\tonly_fingertable_lookups\n");
                                    sendFingerTableLookup();
                                }
                            }
// CASE 1d : reply
                            else if(header.reply){
// CASE 1d1 : reply for fingertable lookup
                                if(only_fingertable_lookups){
                                    printf("CASE 1d1 : reply for fingertable lookup\n");
                                    sa.sin_addr.s_addr = ntohl(header.ip);
                                    inet_ntop(AF_INET, &(sa.sin_addr), fingertable[ft_counter].ip, INET_ADDRSTRLEN);
                                    snprintf(fingertable[ft_counter].port, 7, "%d", header.port);
                                    // printf("header.reply: updated fingertable at %d ip %s port %s\n\n",ft_counter,fingertable[ft_counter].ip,fingertable[ft_counter].port);
                                    fingertable[ft_counter].nodeID = header.id;
                                    ft_counter++;
                                    if(only_fingertable_lookups){
                                        // printf("sendFingerTableLookup for hash %d ft_counter %d\n\n",fingertable[ft_counter].start,ft_counter);
                                        sendFingerTableLookup();
                                        printf(" ------------------ sendfingerTable -------------\n\n");
                                    }
// CASE 1d2 : reply for packet request
                                }else{
                                    terminate_connection(newfd);
                                    printf("CASE 1a : header.reply\n");
                                    snprintf(portstr, 7, "%d", header.port);
                                    sa.sin_addr.s_addr = ntohl(header.ip);
                                    inet_ntop(AF_INET, &(sa.sin_addr), ip4str, INET_ADDRSTRLEN);
                                    // printf("connecting to %s with port %s\n",ip4str,portstr);
                                    newfd = connect_to(ip4str, portstr);
                                    if (newfd == -1) {
                                        printf("Can't connect to a peer.\n"); free(safe_package); intHandler();
                                    }
                                    if(!FD_ISSET(newfd, &master)){
                                        FD_SET(newfd,&master);
                                        if(newfd>fdmax) fdmax = newfd+1;
                                    }
                                    status = send_data(newfd, safe_package, safe_nbytes);
                                    free(safe_package);
                                    if (status == -1) printf("Can't answer client.\n");
                                    if (status != safe_nbytes) printf("Not sended everything. status %d nbytes %d\n",status,safe_nbytes);
                                }
// CASE 1e : lookup request
                            }else if(header.lookup){
                                printf("CASE 1b : header.lookup\n");
// CASE 1e1 : successor responsible
                                if ((header.hashID <= sucID && header.hashID > myID) || (myID > sucID && (header.hashID<= sucID || header.hashID > myID)))
                                {
                                    printf("CASE 1b_1 : Successor responsible\n");
                                    header.reply = 1;
                                    header.lookup = 0;
                                    header.stabilize = 0;
                                    struct sockaddr_in sin;
                                    socklen_t len = sizeof(sin);
                                    if (getsockname(newfd, (struct sockaddr *)&sin, &len) == -1)
                                        perror("getsockname");
                                    if(ntohs(sin.sin_port) !=  atoi(argv[3]))
                                        terminate_connection(newfd);
                                    snprintf(portstr, 7, "%d", header.port);
                                    sa.sin_addr.s_addr = header.ip;
                                    inet_ntop(AF_INET, &(sa.sin_addr), ip4str, INET_ADDRSTRLEN);
                                    
                                    newfd = connect_to(ip4str, portstr);
                                    header.id = sucID;
                                    header.ip = htonl(sucIP);
                                    header.port = htons(sucPort);
                                    nbytes = marshall_internal_request(&package, header);
                                    status = send_data(newfd, package, nbytes);
                                    if (status == -1) printf("Can't answer client.\n");
                                    if (status != nbytes) printf("Not sended everything.\n");
                                    free(package);
                                    terminate_connection(newfd);
// CASE 1e2 : successor not responsible
                                }else{
                                    printf("CASE 1b_2 : Successor not responsible.. forwarding internal message\n");
// CASE 1e2_1 : check in fingertable
                                    if(has_finger_table){
                                        int k = get_peer(fingertable, header.hashID);
                                        strncpy(ip4str, fingertable[k].ip, INET_ADDRSTRLEN);
                                        strncpy(portstr, fingertable[k].port, 8);
                                        // printf("get_peer : k %d ip %s \t port%s \tnode %d\n",k,fingertable[k].ip,fingertable[k].port,fingertable[k].nodeID);
                                        newfd = connect_to(ip4str, portstr);
                                    }
                                    else{
                                        newfd = connect_to(sucIP_str, sucPort_str);
                                    }
                                    if (newfd == -1) {
                                        printf("Can't connect to next peer.\n");
                                        free(buf); intHandler();
                                    }
                                    status = send_data(newfd, buf, nbytes);
                                    if (status == -1) printf("Can't forward to Peer.\n");
                                    if (status != nbytes) printf("Not sended everything.\n");
                                    terminate_connection(newfd);
                                }
                            }
                        }else{
// CASE 2 : external        
                            printf("CASE 2 EXTERN\n");
                            key = (char *) buf + 7;
                            header.hashID = calc_hash(key, header.key_size, HASHSIZE);
// CASE 2a : response from Peer
                            if(header.ack){
                                printf("CASE 2a : RESPONSE FROM PEER.. forwarding back to client\n");
                                if(safe_fd){
                                    status = send_data(safe_fd, buf, nbytes);
                                }else{
                                    newfd = get_sockfd(safe_hash);
                                    status = send_data(newfd, buf, nbytes);
                                }
                                if (status == -1) printf("Can't answer Client.\n");
                                if (status != nbytes) printf("hasnt sent everything.\n");
                                terminate_connection(newfd);
                                if(safe_fd)
                                    terminate_connection(safe_fd);
                                delete_elem(safe_hash,newfd);
                                delete_elem(safe_hash,safe_fd);
                                fdmax--; safe_hash = 0; safe_fd = 0;
                            }
// CASE 2b : Peer responsible
                            else if ((myID == sucID && myID == preID) || (header.hashID<= myID && header.hashID> preID) || (preID > myID && (header.hashID<= myID || header.hashID> preID)))
                            {
                                printf("CASE 2b : PEER RESPONSIBLE.. EXTERNAL MESSAGE\n");
                                key = calloc(header.key_size,1);
                                memcpy(key, (void *) ((char *) buf + 7), header.key_size);
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
                                status = send_data(newfd, package, nbytes);
                                if (status == -1) printf("Can't answer Client.\n");
                                if (status != nbytes) printf("Not sended everything.\n");
                                free(package);
                                hash_data = NULL;
                                terminate_connection(newfd);
                            }
// CASE 2c : Successor responsible
                            else if ((header.hashID <= sucID && header.hashID > myID) || (myID > sucID && (header.hashID <= sucID || header.hashID > myID)))
                            {   
                                printf("CASE 2c : SUCCESSOR RESPONSIBLE\n");
                                new_elem(header.hashID,newfd);
                                safe_hash = header.hashID;

                                printf("connecting to successor %s %s\n",sucIP_str,sucPort_str);
                                newfd = connect_to(sucIP_str, sucPort_str);
                                if (newfd == -1) {
                                    printf("Can't connect to next peer.\n");
                                    free(buf); intHandler();
                                }
                                if(!FD_ISSET(newfd, &master)){
                                    FD_SET(newfd,&master);
                                    if(newfd>fdmax) fdmax = newfd;
                                }
                                status = send_data(newfd, buf, nbytes);
                                if (status == -1) printf("Can't forward to Peer.\n");
                                if (status != nbytes) printf("Not sended everything.\n");
                            }
                            else {
// CASE 2e : Peer and Suc not responsible
                                printf("CASE 2d : Peer sowie Nachfolger nicht zuständig -> Internal Nachricht\n");
                                if (sucID < 0 || sucID == myID) {
                                    printf("No successor to call. exiting\n");
                                    free(buf);
                                    intHandler();
                                }
                                new_elem(header.hashID,newfd);
                                safe_package = calloc(7+header.key_size+header.value_size,1);
                                memcpy(safe_package,buf,7+header.key_size+header.value_size);
                                safe_nbytes = 7 + header.key_size + header.value_size;
                                safe_hash = header.hashID;
                                safe_fd = newfd;
                                set_header_cmd(1,0,0,0,0,0,1);
                                header.hashID = safe_hash;
                                header.id = myID;
                                header.ip = myIP;
                                header.port = myport;
                                nbytes = marshall_internal_request(&package, header);
// CASE 2e1 : check in fingertable
                                if(has_finger_table){
                                    int k = get_peer(fingertable, header.hashID);
                                    strncpy(ip4str, fingertable[k].ip, INET_ADDRSTRLEN);
                                    strncpy(portstr, fingertable[k].port, 8);
                                    printf("get_peer : k %d ip %s \t port%s \tnode %d\n",k,fingertable[k].ip,fingertable[k].port,fingertable[k].nodeID);
                                    newfd = connect_to(ip4str, portstr);
                                }else{
                                        newfd = connect_to(sucIP_str, sucPort_str);
                                }
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
                                    terminate_connection(newfd);
                            }
                        }
                        printf("preID %d\tmyID %d\tsucID %d\n\n",preID,myID,sucID);
                        free(buf);
                    } // END handle receive
                } // END handle data from client
            } // END got new incoming connection
        } // END looping through file descriptors
        curr_time = time(NULL);
        if(curr_time - start_time >= 2){
            if(myID != sucID) send_stabilize();
            start_time = time(NULL);
        }
    } // END main loop
    return 0;
}

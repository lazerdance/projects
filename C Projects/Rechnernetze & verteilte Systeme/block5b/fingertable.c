/*
    Rechnernetze und Verteilte Systeme - Block 5; Aufgabe 5;
    Gruppe: T18G01
*/

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>

#include "protokoll.h"
#include "fingertable.h"

#ifndef HASHSIZE
#define HASHSIZE 65536
#endif
#ifndef FINGERSIZE
#define FINGERSIZE 16
#endif


int power_of_two(int n) {
    int rv = 1;
    for (int i = 0; i < n; i++) rv = rv * 2;
    return rv;
}

void init_fingertable(finger *fingertable, int nodeID, int sucID, char*ip, char *port) {
    for (int i = 0; i < FINGERSIZE; i++) {
        fingertable[i].start = (power_of_two(i) + nodeID) % power_of_two(FINGERSIZE);
        fingertable[i].nodeID = sucID,
        strncpy(fingertable[i].ip, ip, INET_ADDRSTRLEN + 1);
        strncpy(fingertable[i].port, port, 8);
    }
    return;
}

void update_fingertable(finger *fingertable, unsigned int hash, package_header header) {
    int a = 0, b = 0;
    unsigned int start;
    struct sockaddr_in sa;                              // pretend this is loaded with something

    // printf("===\nNode %d hash %d\n", header.id, hash);
    for (int i = 0; i < FINGERSIZE; i++) {
        start = fingertable[i].start;
        b = a && fingertable[i].start < fingertable[(i + 1) % FINGERSIZE].start;
        b = b && start > header.id && fingertable[i].nodeID < header.id;
        if (hash <= header.id)
            a = start >= hash && start <= header.id;
        else
            a = start >= hash || start <= header.id;
        if (a || b) {
            fingertable[i].nodeID = header.id;
            snprintf(fingertable[i].port, 7, "%d", ntohs(header.port));
            sa.sin_addr.s_addr = header.ip;
            inet_ntop(AF_INET, &(sa.sin_addr), fingertable[i].ip, INET_ADDRSTRLEN);
        }
        // printf("Eintrag %d Start %d a/b %d/%d\n", i, fingertable[i].start, a, b);
    }
    // debug_fingertable(fingertable);
    return;
}

void get_peer(char *ip4str, char *port, finger *fingertable, unsigned int hash) {
    unsigned int k = 0;
    for (int i = 0; i < FINGERSIZE; i++) {
        if (fingertable[i].nodeID > fingertable[k].nodeID && hash > (unsigned int) fingertable[i].nodeID)
            k = i;
    }
    strncpy(ip4str, fingertable[k].ip, INET_ADDRSTRLEN);
    strncpy(port, fingertable[k].port, 8);
    return;
}

void debug_fingertable(finger *fingertable) {
    printf("================fingertable===============\n");
    for (int i = 0; i < FINGERSIZE; i++) {
        printf("Eintrag %d Start %d Node %d\n", i, fingertable[i].start, fingertable[i].nodeID);
    }
    return;
}
/*
    Rechnernetze und Verteilte Systeme - Block 6; Aufgabe 5;
    Gruppe: T18G01
*/

#include <time.h>
#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

#include "memory.h"
#include "ntppacket.h"
#include "ntpserver.h"
#include "ntprotocol.h"

void *myalloc(const unsigned int size) {
    void *memory = calloc(size, 1);
    if (!memory) {
        printf("Can't allocate more memory. Exiting.\n");
        exit(1);
    }
    return memory;
}

int main(const int argc, const char* argv[]) {
    if (argc < 2) {
        printf("Das Programm sollte mit mindestens einem Hostname aufgerufen werden:\n./ntpclient hostnames ...\n");
        exit(1);
    }

    ntp_server sact;
    int best_index = 0;
    struct timespec zeit;
    ntp_server server[argc -1];
    uint64_t smallest_sum = 0xffffffffffffffff;

    for (int i = 1; i < argc; i++) {
        memset(server[i - 1].hostname, 0, 100);
        strncpy(server[i - 1].hostname, argv[i], strlen(argv[i]));
        poll_server(argv[i], &server[i - 1]);
    }

    printf("{Hostname} {Root Dispersion (Avg.)} {Dispersion} {Delay (Avg.)} {Offset (Avg.)}\n");
    for (int i = 0; i < argc - 1; i++) {
        sact = server[i];
        printf("{%s} {%.6f} {%.6f} {%.6f} {%.6f}\n",
            sact.hostname,
            (double) sact.root_dispersion / NANO_IN_SEC, (double) sact.dispersion / NANO_IN_SEC,
            (double) sact.delay / NANO_IN_SEC, (double) sact.offset / NANO_IN_SEC);
        if (sact.dispersion + sact.root_dispersion < smallest_sum) {
            best_index = i;
            smallest_sum = sact.dispersion + sact.root_dispersion;
        }
    }

    timespec_get(&zeit, TIME_UTC);

    printf("\nBester Server: %s;\nOffset (Avg.): %.6f\n", server[best_index].hostname,
        (double) server[best_index].offset / NANO_IN_SEC);
    printf("Alte Zeit: %s\t\tmit %ld ns\n", ctime(&zeit.tv_sec), zeit.tv_nsec);

    zeit.tv_nsec += server[best_index].offset;
    while (zeit.tv_nsec > NANO_IN_SEC) {
        zeit.tv_sec += 1;
        zeit.tv_nsec -= NANO_IN_SEC;
    }
    printf("Neue Zeit: %s\t\tmit %ld ns\n", ctime(&zeit.tv_sec), zeit.tv_nsec);

    return 0;
}

/*
    Rechnernetze und Verteilte Systeme - Block 6; Aufgabe 6;
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

    FILE *fpout;
    ntp_server sact;
    ntp_server server[argc -1][MEASUREMENTS];

    for (int i = 1; i < argc; i++) {
        printf("Poll Server: %s\n", argv[i]);
        poll_server(argv[i], server[i - 1]);
    }

    fpout = fopen("measurements.txt", "w");
    if(fpout == NULL) {
        printf("Datei konnte nicht geoeffnet werden.\n");
        exit(1);
    }

    fprintf(fpout, "Hostname;Offset;Delay;rootDispersion\n");
    for (int i = 0; i < argc - 1; i++) {
        for (int j = 0; j < MEASUREMENTS; j++) {
            sact = server[i][j];
            fprintf(fpout, "%s;%.6f;%.6f;%.6f\n",
                sact.hostname,
                (double) sact.offset / NANO_IN_SEC, (double) sact.delay / NANO_IN_SEC,
                (double) sact.root_dispersion / NANO_IN_SEC);
        }
    }
    fclose(fpout);
    return 0;
}

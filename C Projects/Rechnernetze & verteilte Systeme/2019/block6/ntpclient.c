#include <time.h>
#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

#include "ntppacket.h"
#include "ntpserver.h"
#include "ntprotocol.h"


int main(const int argc, const char* argv[]) {
    if (argc < 3) {
        printf("Das Programm sollte mit Anzahl von Requests und mindestens einem Hostname aufgerufen werden:\n./ntpclient Requests hostnames ...\n");
        exit(1);
    }

    int requests = atoi(argv[1]);
    printf("number of requests : %d\n",requests);
    FILE *fpout;
    ntp_server sact;
    ntp_server server[argc -1][requests];

    for (int i = 2; i < argc; i++) {
        printf("Poll Server: %s\n", argv[i]);
        poll_server(argv[i], server[i - 1],requests);
    }

    for (int i = 1; i < argc-1; i++) {
        char fname[100] = {0};
        strcat(fname,argv[i+1]);
        strcat(fname,".txt");
        fpout = fopen(fname, "w");
        printf("opened file %s\n",fname);
        if(fpout == NULL) {
            printf("Datei konnte nicht geoeffnet werden.\n");
            exit(1);
        }
        fprintf(fpout, "Hostname;RequestNumber;rootDispersion;DispOver8Req;Delay;Offset\n");
        for (int j = 0; j < requests; j++) {
            sact = server[i][j];
            fprintf(fpout, "%s;%d;%.6f;%.6f;%.6f;%.6f\n",
                sact.hostname, j,(double) sact.root_dispersion / NANO_IN_SEC,
                (double) sact.dispersion_over_8 / NANO_IN_SEC,
                (double) sact.delay / NANO_IN_SEC,
                (double) sact.offset / NANO_IN_SEC
            );
        }
        fclose(fpout);
    }
    return 0;
}

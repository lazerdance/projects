/*
    Rechnernetze und Verteilte Systeme - Block 2; Aufgabe 6;
    Gruppe: T18G01
*/

#define BUFSIZE 512                         // Groesse fuer den Buffer der empfangenen Daten

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>

int main(const int argc, const char* argv[]) {
    // Kontrolliere ob Eingabeparameter vorhanden sind
    if (argc != 3) {
        printf("Das Programm sollte wie folgt aufgerufen werden:\n./program hostname port\n");
        exit(1);
    }
    // DEBUGGING: Gebe Eingabeparameter aus
    // printf("Eingabeparameter:\n%s, %s\n", argv[1], argv[2]);

    // Variablen deklarieren
    int status;                             // rueckgabewert von getaddrinfo()
    int socket_fd;                          // socket file discriptor
    int byte_amount;                        // rueckgabewert von receive: bytes die empfangen wurden
    char buffer[BUFSIZE];                   // buffer fuer die empfangenen Daten
    struct addrinfo hints;                  // addrinfo um verbindungsparameter fetzulegen
    struct addrinfo *servinfo;              // addrinfo liste fuer rueckgabe von getaddrinfo
    struct addrinfo *p;                     // addrinfo pointer um ueber liste fuer rueckgabe zu iterieren
    struct timespec start, end;             // start- und endzeit für zeitmessung

    // Variablen initieren
    memset(&hints, 0, sizeof (hints));
    clock_gettime(CLOCK_REALTIME, &start);  //zeitstempel vor verbindungsaufbau

    // Vorgaben fuer getaddrinfo
    hints.ai_family = AF_UNSPEC;            // IPv4 oder IPv6
    hints.ai_socktype = SOCK_STREAM;        // Verwende TCP

    // getaddrinfo call + fehlerbehandlung
    if ((status = getaddrinfo(argv[1], argv[2], &hints, &servinfo)) != 0) {
        printf("getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }

    // iteriere durch die rueckgaben von getaddrinfo
    for (p = servinfo; p != NULL; p = p->ai_next) {
        // probiere socket mit den daten von getaddrinfo zu deklarieren
        if ((socket_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            printf("Kann kein socket erstellen... probiere weiter...\n");
            continue;
        }

        // probiere ueber socket zu verbinden
        if (connect(socket_fd, p->ai_addr, p->ai_addrlen) == -1) {
            printf("Kann sich nicht Verbinden... probiere weiter...\n");
            close(socket_fd);
            continue;
        }

        break;                              // wenn verbunden, breche die schleife ab
    }

    freeaddrinfo(servinfo);                 // speicher freigeben

    // Pruefe ob eine Verbindung hergestellt wurde.
    if (p == NULL) {
        printf("Kann keine Verbindung zum Server aufnehmen.\n");
        exit(1);
    }

    // probiere Daten zu empfangen
    while ((byte_amount = recv(socket_fd, buffer, BUFSIZE - 1, 0)) != 0) {
        // fehlerbehandlung bei der Datenuebertragung
        if (byte_amount == -1) {
            printf("Fehler bei der Datenübertragung vom Server.\n");
            close(socket_fd);                   // socket schließen
            exit(1);
        }

        buffer[byte_amount] = '\0';         // setze String Ende (Nullbyte) hinter den empfangenen Bytes
        printf("%s", buffer);               // gebe empfangene Daten aus

        if (byte_amount < BUFSIZE - 1) {
            break;
        }
    }

    close(socket_fd);                       // socket schließen

    clock_gettime(CLOCK_REALTIME, &end);   //zeitstempel nach verbindungsaufbau

    //differenz berechnung ende-start
    int diffsec = end.tv_sec - start.tv_sec;
    long diffnano = (end.tv_nsec - start.tv_nsec);
    long double zeitstempel = diffsec + ((long double) diffnano / 1000000000);
    printf("%LG\n", zeitstempel);

    return 0;                               // beende das Programm
}

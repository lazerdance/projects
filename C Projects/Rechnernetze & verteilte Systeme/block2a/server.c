/*
    Rechnernetze und Verteilte Systeme - Block 2; Aufgabe 5;
    Gruppe: T18G01
*/

#define BACKLOG 5                           // maximale verbindungen die auf accept() warten
#define MAX_LINE_LENGTH 512                 // maximale quote laenge

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>

// globale Variablen um Speicher ordentlich freizugeben
int socket_fd;                              // socket file discriptor
int quote_amount;                           // anzahl der zitate, die eingelesen wurden
char **quotes = NULL;                       // zitate, die eingelesen wurden
struct addrinfo *servinfo;                  // addrinfo liste fuer rueckgabe von getaddrinfo

void intHandler() {
    close(socket_fd);                       // socket schliessen
    // speicher von quotes freigeben
    for (int i = 0; i < quote_amount; i++) {
        free(quotes[i]);
    }
    free(quotes);

    printf("\n");
    exit(0);
}

// lese zitate aus der datei in speicher ein und gebe anzahl der zitate zurueck
int readQuotes(const char *filename) {
    // Variablen deklarieren
    int line_counter = 0;                   // zaehler um die anzahl der 
    char line[MAX_LINE_LENGTH];             // buffer fuer die zeilen

    // probiere Datei mit Zitaten zu oeffnen
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Kann Datei mit Zitaten nicht oeffnen.\n");
        exit(1);
    }

    // lese zeilenweise und zaehle die zeilen/zitate
    while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
        // erweitere speicher fuer neue zeile/neues zitat
        quotes = realloc(quotes, ++line_counter * sizeof(*quotes));
        quotes[line_counter - 1] = malloc(MAX_LINE_LENGTH * sizeof(char));
        // schaue ob der speicher korrekt zugewiesen wurde
        if (quotes == NULL || quotes[line_counter - 1] == NULL) {
            printf("Kann keinen Speicher mehr allozieren.");
            exit(1);
        }
        // speicher vom line buffer in den eigentlichen speicher
        strncpy(quotes[line_counter - 1], line, MAX_LINE_LENGTH);
    }
    // schliesse datei
    fclose(fp);
    // gebe anzahl der gelesenen quotes zurueck
    return line_counter;
}

int main(const int argc, const char* argv[]) {
    // Kontrolliere ob Eingabeparameter vorhanden sind
    if (argc != 3) {
        printf("Das Programm sollte wie folgt aufgerufen werden:\n./program port datei.txt\n");
        exit(1);
    }

    // Kontrolliere ob Port in Ordnung ist
    long port = atol(argv[1]);
    if(port < 0 || port > 65535) {
        perror("Bitte einen Port zwischen 0 und 65535 angeben.");
        exit(1);
    }

    signal(SIGINT, intHandler);

    // Variablen deklarieren
    time_t t;                               // benoetigt fuer zufallszahlen
    int yes = 1;                            // Wert fuer setsockopt()
    int status;                             // rueckgabewert von getaddrinfo() oder gesendete bits
    int randnr;                             // zufallszahl fuer zufaelliges zitat
    int len_msg;                            // laenge der Nachricht, die verschickt werden soll
    int connect_socket;                     // socket fuer verbindung zum remote
    struct addrinfo hints;                  // addrinfo um verbindungsparameter fetzulegen
    struct addrinfo *p;                     // addrinfo pointer um ueber liste fuer rueckgabe zu iterieren
    struct sockaddr_storage remote_addr;    // adressen vom remote
    socklen_t sin_size;                     // groessen von adressen von remote

    // Variablen initieren
    sin_size = sizeof(remote_addr);
    memset(&hints, 0, sizeof (hints));

    srand((unsigned) time(&t));             // zufallszahlengenerator initialisieren

    // lese quotes ein
    quote_amount = readQuotes(argv[2]);

    hints.ai_family = AF_UNSPEC;            // IPv4 oder IPv6
    hints.ai_socktype = SOCK_STREAM;        // Verwende TCP
    hints.ai_flags = AI_PASSIVE;            // eigene IP automatisch eintragen

    // getaddrinfo call + fehlerbehandlung
    if ((status = getaddrinfo(NULL, argv[1], &hints, &servinfo)) != 0) {
        printf("getaddrinfo error: %s\n", gai_strerror(status));
        // speicher von quotes freigeben
        for (int i = 0; i < quote_amount; i++) {
            free(quotes[i]);
        }
        free(quotes);
        exit(1);
    }

    // iteriere durch die rueckgaben von getaddrinfo
    for (p = servinfo; p != NULL; p = p->ai_next) {
        // probiere socket mit den daten von getaddrinfo zu deklarieren
        if ((socket_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            printf("Kann kein socket erstellen... probiere weiter...\n");
            continue;
        }

        // Port nur blockieren, wenn auf dem Port aktiv gelauscht wird
        if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            printf("Kann Port Optionen nicht setzen.\n");
            freeaddrinfo(servinfo);             // speicher freigeben
            close(socket_fd);                   // socket schliessen
            // speicher von quotes freigeben
            for (int i = 0; i < quote_amount; i++) {
                free(quotes[i]);
            }
            free(quotes);
            exit(1);
        }

        // versuche socket an Port zu binden
        if (bind(socket_fd, p->ai_addr, p->ai_addrlen) == -1) {
            printf("Kann socket nicht an den Port binden.\n");
            close(socket_fd);
            continue;
        }

        break;                              // wenn verbunden, breche die schleife ab
    }

    freeaddrinfo(servinfo);                 // speicher freigeben

    // Pruefe ob der Port gebunden wurde
    if (p == NULL) {
        printf("Kann Port nicht binden.\n");
        close(socket_fd);                   // socket schliessen
        // speicher von quotes freigeben
        for (int i = 0; i < quote_amount; i++) {
            free(quotes[i]);
        }
        free(quotes);
        exit(1);
    }

    // fange an auf dem port zu lauschen; maximal 5 remotes gleichzeitig
    if (listen(socket_fd, BACKLOG) == -1) {
        printf("Kann auf dem Port nicht hoeren.\n");
        close(socket_fd);                   // socket schliessen
        // speicher von quotes freigeben
        for (int i = 0; i < quote_amount; i++) {
            free(quotes[i]);
        }
        free(quotes);
        exit(1);
    }

    // dauerschleife um immer wieder zitate zu verschicken, bei STRG-C wird keepRunning = 0
    while (1) {
        // warte auf Verbindung und akzeptiere
        if ((connect_socket = accept(socket_fd, (struct sockaddr*) &remote_addr, &sin_size)) == -1) {
            continue;
        }

        randnr = rand() % quote_amount;
        len_msg = strlen(quotes[randnr]);
        status = send(connect_socket, quotes[randnr], len_msg, 0);
        if (status == -1) {
            printf("Konnte Daten nicht uebertragen.\n");
        } else if (status != len_msg) {
            printf("Konnte nicht die gesamte Nachricht verschicken.\n");
        }
        
        close(connect_socket);              // remote socket freigeben
    }

    return 0;
}

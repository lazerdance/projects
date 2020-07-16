/*
    Rechnernetze und Verteilte Systeme - Block 2; Aufgabe 7;
    Gruppe: T18G01
*/

#define BUFSIZE 1024                            // Groesse fuer den Buffer der empfangenen Daten

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

char *build_http_request(char *domain, char *path) {
    // http header zusammen bauen: speicher 50 fuer andere zeichen + laenge domain/path
    char *http = malloc((50 + strlen(domain) + strlen(path)) * sizeof(char));
    strcpy(http, "GET /");
    strcat(http, path);
    strcat(http, " HTTP/1.0\r\nHOST: ");
    strcat(http, domain);
    strcat(http, "\r\nConnection: close\r\n\r\n");
    return http;
}

int create_connection(struct addrinfo *servinfo) {
    int socket_fd;                              // socket file discriptor
    struct addrinfo *p;                         // addrinfo pointer um ueber liste fuer rueckgabe zu iterieren

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

        break;                                  // wenn verbunden, breche die schleife ab
    }

    // Pruefe ob eine Verbindung hergestellt wurde.
    if (p == NULL) {
        return -1;
    }

    return socket_fd;
}

int receive_data(int socket_fd, char **datapointer) {
    int bytes_recv;                            // rueckgabewert von receive: bytes die empfangen wurden
    int bytes_total = 0;
    // buffer fuer die empfangenen Daten
    char *data = calloc(1, sizeof(char));
    char *buffer = calloc(BUFSIZE, sizeof(char));

    // probiere Daten zu empfangen
    while ((bytes_recv = recv(socket_fd, buffer, BUFSIZE, 0)) != 0) {
        // fehlerbehandlung bei der Datenuebertragung
        if (bytes_recv == -1) {
            free(data);
            free(buffer);
            return -1;
        }

        bytes_total += bytes_recv;
        data = realloc(data, (bytes_total + 1) * sizeof(char));
        memcpy(data + bytes_total - bytes_recv, buffer, bytes_recv);

    }
    free(buffer);

    *datapointer = data;
    return bytes_total;
}

int main(const int argc, const char* argv[]) {
    // Kontrolliere ob Eingabeparameter vorhanden sind
    if (argc != 2) {
        printf("Das Programm sollte wie folgt aufgerufen werden:\n./program [http-adresse]\n");
        exit(1);
    }

    // Variablen deklarieren
    int status;                                 // rueckgabewert von getaddrinfo()
    int socket_fd;                              // socket file discriptor
    int byte_amount;                            // anzahl gesendete/empfangene bytes
    char *domain, *path;                        // adresse auseinandernehmen
    char *data_recv, *http_request, *data;      // daten senden und empfangen; zeiger auf daten nach http header
    struct addrinfo hints;                      // addrinfo um verbindungsparameter fetzulegen
    struct addrinfo *servinfo;                  // addrinfo liste fuer rueckgabe von getaddrinfo

    // Variablen initieren
    memset(&hints, 0, sizeof (hints));
    path = calloc(200, sizeof(char));
    domain = calloc(200, sizeof(char));

    sscanf(argv[1], "http://%199[^/]/%199[^\n]", domain, path);
    if (strlen(domain) == 0 || strlen(path) == 0) {
        printf("Bitte gueltige http-adresse angeben!\n");
        free(path);
        free(domain);
        exit(1);
    }

    // Vorgaben fuer getaddrinfo
    hints.ai_family = AF_UNSPEC;                // IPv4 oder IPv6
    hints.ai_socktype = SOCK_STREAM;            // Verwende TCP

    // getaddrinfo call + fehlerbehandlung
   if ((status = getaddrinfo(domain, "http", &hints, &servinfo)) != 0) {
        printf("getaddrinfo error: %s\n", gai_strerror(status));
        free(path);
        free(domain);
        exit(1);
    }

    // socket erstellen und verbinden
    if ((socket_fd = create_connection(servinfo)) == -1) {
        printf("Kann keine Verbindung zum Server aufnehmen.\n");
        free(path);
        free(domain);
        freeaddrinfo(servinfo);
        exit(1);
    }

    freeaddrinfo(servinfo);                     // speicher freigeben

    // http anfrage zum senden bauen
    http_request = build_http_request(domain, path);
    byte_amount = send(socket_fd, http_request, strlen(http_request), 0);
    if (byte_amount == -1) {
        printf("Kann HTTP Anfrage nicht senden.\n");
        free(path);
        free(domain);
        free(http_request);
        close(socket_fd);
        exit(1);
    }

    free(http_request);                         // speicher freigabe

    // Daten empfangen und ausgeben
    if((byte_amount = receive_data(socket_fd, &data_recv)) == -1) {
        printf("Fehler bei der Datenübertragung vom Server.\n");
        free(path);
        free(domain);
        close(socket_fd);
        exit(1);
    }
    // daten vom header abtrennen
    if((data = strstr(data_recv, "\r\n\r\n")) == NULL) {   
        printf("Kann HTTP Header nicht von Daten trennen.\n");
        free(path);
        free(domain);
        free(data_recv);
        close(socket_fd);
        exit(1);
    }
    data += 4;

    // printf("%d\n", byte_amount);
    fwrite(data, sizeof(char), byte_amount - (data - data_recv), stdout);

    // speicherfreigabe
    free(path);
    free(domain);
    free(data_recv);

    close(socket_fd);                           // socket schließen

    return 0;                                   // beende das Programm
}

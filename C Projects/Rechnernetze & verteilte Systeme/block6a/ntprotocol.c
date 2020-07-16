/*
    Rechnernetze und Verteilte Systeme - Block 6; Aufgabe 5;
    Gruppe: T18G01
*/

#include <stdio.h>
#include <time.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "memory.h"
#include "ntppacket.h"
#include "ntpserver.h"
#include "ntprotocol.h"


void calc_avg(uint64_t *delay, int64_t *offset, ntp_server *server_data) {
    uint64_t delay_max = 0, delay_min = 0, nano;

    nano = server_data->root_dispersion_avg & 0xffff;
    nano *= NANO_IN_SEC;
    server_data->root_dispersion = nano >> 16;
    server_data->root_dispersion += (server_data->root_dispersion_avg >> 16) * NANO_IN_SEC;

    server_data->delay= 0;
    server_data->offset = 0;

    for (int i = 0; i < 8; i++) {
        server_data->delay += delay[i];
        server_data->offset += offset[i];
        if (delay_max < delay[i]) delay_max = delay[i];
        else if (delay_min > delay[i]) delay_min = delay[i];
    }
    server_data->delay >>= 3;
    server_data->offset >>= 3;

    server_data->dispersion = delay_max - delay_min;
    return;
}

void calculate_offset_delay(uint64_t t1, uint64_t t4, ntp_packet *answer, uint64_t *delayrv, int64_t *offsetrv) {
    int64_t offset;
    uint64_t t2, t3, delay, nano;
    struct timespec t2_stamp, t3_stamp;

    t2_stamp.tv_sec = ntohl(answer->rxTm_s) - NTP_UNIX_OFFSET;
    nano = ntohl(answer->rxTm_f);
    t2_stamp.tv_nsec = (nano * NANO_IN_SEC) >> 32;
    t3_stamp.tv_sec = ntohl(answer->txTm_s) - NTP_UNIX_OFFSET;
    nano = ntohl(answer->txTm_f);
    t3_stamp.tv_nsec = (nano * NANO_IN_SEC) >> 32;

    t2 = t2_stamp.tv_sec * NANO_IN_SEC + t2_stamp.tv_nsec;
    t3 = t3_stamp.tv_sec * NANO_IN_SEC + t3_stamp.tv_nsec;

    delay = (t4 - t1) - (t3 - t2);
    offset = (t2 - t1) + (t3 - t4);

    *delayrv = delay >> 1;
    *offsetrv = offset >> 1;
    return;
}

int get_packets(int socket, struct addrinfo *addr, ntp_server *server_data) {
    fd_set readfds;
    int byte_amount;
    int64_t offset[8];
    struct timeval tv;
    ntp_packet *answer;
    uint64_t t1, t4, delay[8];
    uint32_t root_dispersion_sum = 0;
    struct timespec t1_stamp, t4_stamp;
    ntp_packet request = {35, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    FD_ZERO(&readfds);
    FD_SET(socket, &readfds);
    tv.tv_sec = 1;
    tv.tv_usec = 0;

    for (int j = 0; j < 8; j++) {
        timespec_get(&t1_stamp, TIME_UTC);

        if((byte_amount = sendto(socket, &request, sizeof(ntp_packet), 0, addr->ai_addr, addr->ai_addrlen)) == -1)
            return -1;
        answer = myalloc(sizeof(ntp_packet));
        select(socket + 1, &readfds, NULL, NULL, &tv);
        if (FD_ISSET(socket, &readfds))
            byte_amount = recvfrom(socket, answer, sizeof(ntp_packet), 0, addr->ai_addr, &addr->ai_addrlen);
        else byte_amount = 0;

        timespec_get(&t4_stamp, TIME_UTC);

        if (byte_amount != sizeof(ntp_packet)) {
            printf("Fehler bei der Datenübertragung vom Server.\n");
            close(socket);                   // socket schließen
            free(answer);
            return -1;
        }
        t1 = t1_stamp.tv_sec * NANO_IN_SEC + t1_stamp.tv_nsec;
        t4 = t4_stamp.tv_sec * NANO_IN_SEC + t4_stamp.tv_nsec;
        calculate_offset_delay(t1, t4, answer, &delay[j], &offset[j]);
        root_dispersion_sum += ntohl(answer->rootDispersion);
        free(answer);
    }
    server_data->root_dispersion_avg = root_dispersion_sum >> 3;
    calc_avg(delay, offset, server_data);
    close(socket);
    return 0;
}

void poll_server(const char *ip, ntp_server *server_data) {
    int rv, socket_fd;
    struct addrinfo hints;
    struct addrinfo *servinfo, *p;

    // Variablen initieren
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;            // IPv4 oder IPv6
    hints.ai_socktype = SOCK_DGRAM;         // Verwende TCP

    if ((rv = getaddrinfo(ip, "123", &hints, &servinfo)) != 0) {
        printf("getaddrinfo error: %s\n", gai_strerror(rv));
        exit(1);
    }

    for (p = servinfo; p != NULL; p = p->ai_next) {
        // probiere socket mit den daten von getaddrinfo zu deklarieren
        if ((socket_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            printf("Kann kein socket erstellen... probiere weiter...\n");
            continue;
        }
        if (get_packets(socket_fd, p, server_data) == -1) {
            printf("Fehler bei Kommunikation mit Server... probiere weiter...\n");
            continue;
        }
        break;
    }

    if (p == NULL) {
        printf("Can't send request to %s\n", ip);
        freeaddrinfo(servinfo);
        exit(1);
    }

    freeaddrinfo(servinfo);
    return;
}

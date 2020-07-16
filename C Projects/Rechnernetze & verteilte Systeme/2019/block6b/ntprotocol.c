#include <stdio.h>
#include <time.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "ntppacket.h"
#include "ntpserver.h"
#include "ntprotocol.h"

uint64_t delay_rb[8] = {0};
int rb_index = 0;
uint64_t min_delay = 999999999;
uint64_t max_delay = 0;
uint64_t curr_disp_over_8 = 0;
int requests = 0;
const char *current_ip;

void add_delay_to_buffer(uint64_t delay){
    if(rb_index<8){
        delay_rb[rb_index++] = delay;
    }else{
        int pre=0;
        for(int post=1; post<8; post++){
            delay_rb[pre++] = delay_rb[post++];
        }
        delay_rb[rb_index%8] = delay;
        rb_index++;
    }
    for(int x=rb_index-1; x>rb_index-8; x--){
        if(x<0) break;
        if(min_delay > delay_rb[x%8]) min_delay = delay_rb[x%8];
        if(max_delay < delay_rb[x%8]) max_delay = delay_rb[x%8];
    }
    curr_disp_over_8 = max_delay - min_delay;
    max_delay = 0; min_delay = 999999999;
}

void calc_root_dispersion(uint32_t root_dispersion, uint64_t *root_dispersion_out) {
    uint64_t nano = (root_dispersion & 0xffff) * NANO_IN_SEC;
    *root_dispersion_out = nano >> 16;
    *root_dispersion_out += (root_dispersion >> 16) * NANO_IN_SEC;
    return;
}

void calc_offset_delay(uint64_t t1, uint64_t t4, ntp_packet *answer, uint64_t *delayrv, int64_t *offsetrv, uint64_t *dispersion_over_8) {
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
    add_delay_to_buffer(delay);
    *delayrv = delay >> 1;
    *dispersion_over_8 = curr_disp_over_8;
    *offsetrv = offset >> 1;
    return;
}

void create_packet(ntp_packet *request) {
    uint64_t nano;
    struct timespec timestamp;
    timespec_get(&timestamp, TIME_UTC);

    nano = timestamp.tv_nsec << 32;
    nano /= NANO_IN_SEC;

    memset(request, 0, sizeof(ntp_packet));
    request->li_vn_mode = 35;
    request->origTm_s = htonl(timestamp.tv_sec + NTP_UNIX_OFFSET);
    request->origTm_f = htonl(nano);
    return;
}

int get_packets(int socket, struct addrinfo *addr, ntp_server *server_data) {

    fd_set readfds;
    int byte_amount;
    uint64_t t1, t4;
    struct timeval tv;
    ntp_packet *answer;
    struct timespec t1_stamp, t4_stamp;
    ntp_packet request; // = {35, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    create_packet(&request);

    FD_ZERO(&readfds);
    FD_SET(socket, &readfds);

    for (int i = 0; i < requests; i++) {
        if (i % 5 == 0) printf("Iteration: %d\n", i);
        memset(server_data[i].hostname, 0, 100);
        strncpy(server_data[i].hostname, current_ip, strlen(current_ip));

        tv.tv_sec = 1;
        tv.tv_usec = 0;

        timespec_get(&t1_stamp, TIME_UTC);

        if((byte_amount = sendto(socket, &request, sizeof(ntp_packet), 0, addr->ai_addr, addr->ai_addrlen)) == -1)
            return -1;
            
        answer = calloc(sizeof(ntp_packet), 1);
        select(socket + 1, &readfds, NULL, NULL, &tv);
        if (FD_ISSET(socket, &readfds))
            byte_amount = recvfrom(socket, answer, sizeof(ntp_packet), 0, addr->ai_addr, &addr->ai_addrlen);
        else byte_amount = 0;

        timespec_get(&t4_stamp, TIME_UTC);

        if (byte_amount != sizeof(ntp_packet)) {
            free(answer);
            close(socket);                   // socket schließen
            return -1;
        }
        t1 = t1_stamp.tv_sec * NANO_IN_SEC + t1_stamp.tv_nsec;
        t4 = t4_stamp.tv_sec * NANO_IN_SEC + t4_stamp.tv_nsec;

        calc_offset_delay(t1, t4, answer, &server_data->delay, &server_data->offset, &server_data->dispersion_over_8);
        calc_root_dispersion(ntohl(answer->rootDispersion), &server_data->root_dispersion);

        free(answer);
        sleep(FREQUENCY);
    }
    close(socket);
    return 0;
}

void poll_server(const char *ip, ntp_server *server_data, int req_numb) {
    current_ip = ip;
    requests = req_numb;
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
        if (get_packets(socket_fd, p, server_data) == -1)
            continue;
        break;
    }

    if (p == NULL) {
        printf("Can't send all requests to %s\n", ip);
        freeaddrinfo(servinfo);
        sleep(FREQUENCY);
    }

    freeaddrinfo(servinfo);

    return;
}

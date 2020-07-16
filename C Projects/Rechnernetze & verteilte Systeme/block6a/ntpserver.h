/*
    Rechnernetze und Verteilte Systeme - Block 6; Aufgabe 5;
    Gruppe: T18G01
*/

#ifndef ntpserver_h
#define ntpserver_h

typedef struct _ntp_server {
    char hostname[100];
    uint32_t root_dispersion_avg;
    uint64_t root_dispersion;
    uint64_t dispersion;
    uint64_t delay;
    int64_t offset;
} ntp_server;

#endif

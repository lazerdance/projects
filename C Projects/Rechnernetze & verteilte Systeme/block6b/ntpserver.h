/*
    Rechnernetze und Verteilte Systeme - Block 6; Aufgabe 6;
    Gruppe: T18G01
*/

#ifndef ntpserver_h
#define ntpserver_h

#define MEASUREMENTS 100
#define FREQUENCY 6

typedef struct _ntp_server {
    char hostname[100];
    int64_t offset;
    uint64_t delay;
    uint64_t root_dispersion;
} ntp_server;

#endif

#ifndef ntpserver_h
#define ntpserver_h

#define FREQUENCY 8

typedef struct _ntp_server {
    char hostname[100];
    int64_t offset;
    uint64_t delay;
    uint64_t root_dispersion;
    uint64_t dispersion_over_8;
} ntp_server;

#endif

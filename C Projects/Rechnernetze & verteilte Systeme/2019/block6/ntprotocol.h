#ifndef ntprotocol_h
#define ntprotocol_h

#define NTP_UNIX_OFFSET 2208988800
#define NANO_IN_SEC 1000000000

void poll_server(const char *ip, ntp_server *server_data, int req_numb);

#endif

/*
    Rechnernetze und Verteilte Systeme - Block 5; Aufgabe 5;
    Gruppe: T18G01
*/

typedef struct _finger {
    int nodeID;
    unsigned int start;
    char port[8];
    char ip[INET_ADDRSTRLEN + 1];
} finger;

void init_fingertable(finger *fingertable, int nodeID, int sucID, char*ip, char *port);
void update_fingertable(finger *fingertable, unsigned int hash, package_header header);
void debug_fingertable(finger *fingertable);
void get_peer(char *ip4str, char *port, finger *fingertable, unsigned int hash);

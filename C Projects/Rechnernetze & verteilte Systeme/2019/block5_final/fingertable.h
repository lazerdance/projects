typedef struct _finger {
    int nodeID;
    unsigned int start;
    char port[8];
    char ip[INET_ADDRSTRLEN + 1];
} finger;

void init_fingertable(finger *fingertable, unsigned short nodeID);
void update_fingertable(finger *fingertable, unsigned int hash, package_header header);
void debug_fingertable(finger *fingertable);
int get_peer(finger *fingertable, unsigned int hash);

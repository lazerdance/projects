/*
    Rechnernetze und Verteilte Systeme - Block 5; Aufgabe 5;
    Gruppe: T18G01
*/

#ifndef list_h
#define list_h

typedef struct _qelement {
    int socket;
    uint8_t transID;
    unsigned int hash;
    uint8_t orgTransID;
    struct _qelement *next;
} qelement;

typedef struct _qhead {
    unsigned int quantity;
    qelement *next;
} qhead;

qhead init_list();
void append_list(qhead *head, int socket, uint8_t transID, uint8_t orgTransID, unsigned int hash);
qelement* find_list(qhead *head, const uint8_t transID);
void remove_transid_list(qhead *head, const uint8_t transID);
void remove_socket_list(qhead *head, const int socket);
void free_list(qhead *head);

#endif

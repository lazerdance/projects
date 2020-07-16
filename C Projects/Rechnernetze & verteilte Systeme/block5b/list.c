/*
    Rechnernetze und Verteilte Systeme - Block 5; Aufgabe 5;
    Gruppe: T18G01
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "list.h"
#include "memory.h"

qhead init_list() {
    qhead list;
    list.next = NULL;
    list.quantity = 0;
    return list;
}

void append_list(qhead *head, int socket, uint8_t transID, uint8_t orgTransID, unsigned int hash) {
    qelement *p, *last;

    qelement *new = myalloc(sizeof(qelement));
    new->socket = socket;
    new->transID = transID;
    new->hash = hash;
    new->orgTransID = orgTransID;
    new->next = NULL;

    if (!head->next) {
        head->next = new;
        head->quantity = head->quantity + 1;
        return;
    }

    p = head->next;
    while (p) {
        last = p;
        p = p->next;
    }
    last->next = new;
    head->quantity = head->quantity + 1;
    return;
}

qelement* find_list(qhead *head, const uint8_t transID) {
    qelement *p = head->next;
    while (p) {
        if (p->transID == transID) {
            return p;
        }
        p = p->next;
    }
    return NULL;
}

void remove_transid_list(qhead *head, const uint8_t transID) {
    qelement *p, *last;
    p = head->next;
    last = NULL;
    while (p) {
        if (p->transID == transID) {
            if (last) {
                last->next = p->next;
                free(p);
                p = last->next;
            } else {
                head->next = p->next;
                free(p);
                p = head->next;
            }
            head->quantity = head->quantity - 1;
        } else {
            last = p;
            p = p->next;
        }
    }
    return;
}

void remove_socket_list(qhead *head, const int socket) {
    qelement *p, *last;
    p = head->next;
    last = NULL;
    while (p) {
        if (p->socket == socket) {
            if (last) {
                last->next = p->next;
                free(p);
                p = last->next;
            } else {
                head->next = p->next;
                free(p);
                p = head->next;
            }
            head->quantity = head->quantity - 1;
        } else {
            last = p;
            p = p->next;
        }
    }
    return;
}

void free_list(qhead *head) {
    qelement *p, *last;
    p = head->next;
    last = head->next;
    while (p) {
        p = p->next;
        free(last);
        last = p;
    }
    head->quantity = 0;
    return;
}

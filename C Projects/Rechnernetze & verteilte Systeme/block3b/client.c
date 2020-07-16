/*
    Rechnernetze und Verteilte Systeme - Block 3; Aufgabe 6;
    Gruppe: T18G01
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "protokoll.h"
#include "connection.h"

void *myalloc(const unsigned int size) {
    void *memory = calloc(size, 1);
    if (!memory) {
        printf("Can't allocate more memory. Exiting.\n");
        exit(1);
    }
    return memory;
}

char *remote_procedure_call(uint8_t operation, const char *key, const char *value) {
    void *answer = NULL;
    void *package = NULL;
    int answer_size = 0;
    int sended_bytes = 0;
    unsigned int package_size = 0;
    package_header header;
    char *get_value = NULL;
    struct timeval tv;

    if (!setup_connection()) {
        printf("Can't connect to host.\n");
        return get_value;
    }

    package_size = marshall_query(&package, operation, key, value);

    while (answer_size < 1) {
        sended_bytes = send_package(package, package_size);
        if (sended_bytes == -1) {
            printf("Konnte Daten nicht uebertragen.\n");
            return get_value;
        }

        tv.tv_sec = 2;
        tv.tv_usec = 0;

        answer_size = receive_data(&answer, &tv);
        if (answer_size == -1) {
            printf("Error while receiving.\n");
            free(package);
            close_connection();
            return get_value;
        } else if (answer_size == 0) {
            close_connection();
            if (!setup_connection()) {
                printf("Can't connect to host.\n");
                free(answer);
                free(package);
                return get_value;
            }
        }
    }

    header = unmarshall(answer);
    protokoll_debug(answer, header);

    if (header.get && header.value_size > 0) {
        get_value = myalloc((header.value_size + 1) * sizeof(char));
        memcpy(get_value, ((char *) answer) + 6 + header.key_size, header.value_size);
    }

    free(answer);
    free(package);
    close_connection();
    return get_value;
}

void set_hashmap(const char *key, const char *value) {
    remote_procedure_call(2, key, value);
    return;
}

char *get_hashmap(const char *key) {
    char *value = remote_procedure_call(4, key, NULL);
    return value;
}

void del_hashmap(const char *key) {
    remote_procedure_call(1, key, NULL);
    return;
}

int main(const int argc, const char* argv[]) {
    char error[] = "Das Programm sollte wie folgt aufgerufen werden:\n./client <DNS-Name> <Port> <SET|GET|DELETE> <KEY> [VALUE]\n";

    // Kontrolliere Eingabeparameter
    if (argc < 4) {
        printf("%s", error);
        exit(1);
    }

    uint8_t set = (strlen(argv[3]) == 3 && !strncmp(argv[3], "SET", 3)) ? 1 : 0;
    uint8_t get = (!set && strlen(argv[3]) == 3 && !strncmp(argv[3], "GET", 3)) ? 1 : 0;
    uint8_t del = (!set && !get && strlen(argv[3]) == 6 && !strncmp(argv[3], "DELETE", 6)) ? 1 : 0;

    if ((!set && !get && !del) || (set && argc != 6) || ((get || del) && argc != 5)) {
        printf("%s", error);
        exit(1);
    }

    pass_host(argv[1], argv[2]);

    if (set) set_hashmap(argv[4], argv[5]);
    else if (del) del_hashmap(argv[4]);
    else if (get) {
        char *value = get_hashmap(argv[4]);
        if (value) {
            printf("%s\n", value);
            free(value);
        }
    }

    free_host();
    return 0;
}

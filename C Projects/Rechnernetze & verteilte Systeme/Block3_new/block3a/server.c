#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include "protocol.h"
#include "hashmap.h"

#define HASHSIZE 128
#define MAX_CLIENTS 5
#define BUFSIZE 512

#define GET_BIT (1<<2)
#define SET_BIT (1<<1)
#define DEL_BIT (1)

int socket_fd;
hash_map hashmap;

void garbageCollector() {
    cleanHashmap(hashmap);
    close(socket_fd);
    exit(0);
    return;
}

int bind_socket(struct addrinfo *servinfo) {
    int f_socket = -1;
    int const yes = 1;                      // yes = setsocketopt()
    struct addrinfo *p;                     // addrinfo pointer

    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((f_socket = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            printf("creating socket failed.. trying next one\n");
            continue;
        }

        // block port when busy in order to avoid collisions
        if (setsockopt(f_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            printf("Problem configuring the socket.\n");
            freeaddrinfo(servinfo);
            close(f_socket);
            exit(1);
        }

        if (bind(f_socket, p->ai_addr, p->ai_addrlen) == -1) {
            printf("Kann socket nicht an den Port binden.\n");
            close(f_socket);
            continue;
        }
        break;
    }
    if (p == NULL) {
        return -1;
    }

    return f_socket;
}

int receive_data(int socket_fd, void **datapointer) {
    int bytes_recv;
    int bytes_total = 0;
    int package_size = 0;
    package_header header;

    void *data = calloc(1,1);
    void *buffer = calloc(BUFSIZE,1);

    while ((bytes_recv = recv(socket_fd, buffer, BUFSIZE, 0)) != 0) {
        if (bytes_recv == -1) {
            free(data);
            free(buffer);
            return -1;
        }

        bytes_total += bytes_recv;
        data = realloc(data, (bytes_total + 1));
        if (data == NULL) {
            printf("Can't allocate more memory. Exiting.\n");
            exit(1);
        }

        memcpy((char *) data + bytes_total - bytes_recv, buffer, bytes_recv);
        if (!package_size) {
            printf("bytes received %d\n",bytes_total);
            header = unmarshall(data);
            package_size = (signed int) (7 + header.key_length + header.value_length);
        }
        if (package_size == bytes_total) break;
        if(bytes_recv < BUFSIZE) break;
    }
    free(buffer);

    *datapointer = data;
    return bytes_total;
}

int send_package(int connected_socket, void *data, unsigned int data_size) {
    int status = send(connected_socket, data, data_size, 0);
    if (status == -1) {
        printf("Error while transferring data.\n");
    } else if (status != (int) data_size) {
        printf("error while sending package\n");
    }
    return status;
}


int main(const int argc, const char* argv[]) {
    if (argc != 2) {
        printf("The program should be called as followed:\n\t\t\t\t./program port\n");
        exit(1);
    }

    long port = atol(argv[1]);
    if(port < 0 || port > 65535) {
        perror("please choose a port between 1 and 65535.");
        exit(1);
    }

    int status;                             // returns status from getaddrinfo() or sent bits
    int connected_socket;                     // our socket to connect
    struct addrinfo hints;                  // addrinfo holding connection parameters
    struct addrinfo *servinfo;              // addrinfo lists for returns from getaddrinfo()
    struct sockaddr_storage remote_addr;    // the remote address to connect to
    socklen_t sin_size;                     // size of remote addresses

    hashmap = create_hashmap(HASHSIZE);
    sin_size = sizeof(remote_addr);
    memset(&hints, 0, sizeof (hints));

    hints.ai_family = AF_UNSPEC;            // IPv4 oder IPv6
    hints.ai_socktype = SOCK_STREAM;        // use TCP
    hints.ai_flags = AI_PASSIVE;            // choose IP passively

    // getaddrinfo call and check
    if ((status = getaddrinfo(NULL, argv[1], &hints, &servinfo)) != 0) {
        printf("getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }

    // bind
    if ((socket_fd = bind_socket(servinfo)) == -1) {
        printf("Kann kein Socket mit gebundenen Port erstellen.\n");
        freeaddrinfo(servinfo);
        exit(1);
    }

    freeaddrinfo(servinfo);
    signal(SIGINT, garbageCollector);             // handler to collect garbage in case of ctrl + c

    // listen
    if (listen(socket_fd, MAX_CLIENTS) == -1) {
        printf("Kann auf dem Port nicht hoeren.\n");
        close(socket_fd);
        exit(1);
    }

    // loop keeping server always on
    while (1) {
        // connect to the first client on the list if not empty
        if ((connected_socket = accept(socket_fd, (struct sockaddr*) &remote_addr, &sin_size)) == -1) {
            printf("accept Error\n");
            continue;
        }
        int bytes_recv;
        unsigned int package_size;
        void *key, *value, *package, *buffer;
        package_header header;
        hash_entry *hash_data = NULL;

        while ((bytes_recv = receive_data(connected_socket, &buffer)) >= 0) {
            if (bytes_recv == -1 && connected_socket) {
                printf("stopped receiving Data\n");
                break;
            }

            header = unmarshall(buffer);
            print_report(buffer, header);

            key = calloc(header.key_length,1);
            memcpy(key, (void *) ((char *) buffer + 7), header.key_length);

            if (header.rcp_cmd & GET_BIT) {
                if(!(hash_data = get(hashmap, key, header.key_length))) header.rcp_cmd &= ~GET_BIT;
            }
            else if (header.rcp_cmd & SET_BIT) {
                value = calloc(header.value_length,1);
                memcpy(value, (void *) ((char *) buffer + 7 + header.key_length), header.value_length);
                set(hashmap, key, header.key_length, value, header.value_length);
            }
            else if (header.rcp_cmd & DEL_BIT) {
                if(!del(hashmap, key, header.key_length))
                header.rcp_cmd &= ~DEL_BIT;
            } else {
                printf("No Action Bit Set\n");
                free(key);  free(buffer); break;
            }

            if (hash_data) package_size = marshall(&package, buffer, hash_data->value, hash_data->value_length, header);
            else package_size = marshall(&package, buffer, NULL, 0, header);

            send_package(connected_socket, package, package_size);
            close(connected_socket);
            hash_data = NULL;
            free(package);
            free(buffer);
        }
        close(connected_socket);
    }
    close(socket_fd);
    cleanHashmap(hashmap);
    return 0;
}

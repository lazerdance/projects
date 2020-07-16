#ifndef BRUTEFORCER_H
#define BRUTEFORCER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include "libs/sha256.h"
#include "helper/hashloader.h"
#include "helper/pwgen.h"

/**
 * Globale Variablen
**/

// Die maximale Länge der Passwörter (ohne Nullbyte), die getestet werden sollen
int pwd_maxlen = 0;

// Die maximale Anzahl gleichzeitig laufender worker
int max_workers = 0;

// Array in dem die PIDs der laufenden worker abgelegt werden
pid_t *worker = NULL;

/**
 * Funktionen
**/
pwd *crack_hash(char *hash);
int test_string(char *string, char *hash);
int update_worker();

/**
 * INFO macro
 * Lässt sich wie printf() bedienen, gibt aber auf stderr in Farbe aus
**/
#define INFO(...) fprintf(stderr, "\033[01;33m"); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\033[0m")

#endif

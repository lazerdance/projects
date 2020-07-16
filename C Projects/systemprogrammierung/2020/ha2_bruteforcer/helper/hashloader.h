#ifndef HASHLOADER_H
#define HASHLOADER_H

#define  _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

typedef struct hashes {
	char **array;
	int len;
} hashes;

hashes *load_hashes(char *filename);
void free_hashes(hashes *loaded_hashes);

#endif

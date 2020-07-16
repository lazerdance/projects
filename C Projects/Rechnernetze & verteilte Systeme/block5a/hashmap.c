/*
    Rechnernetze und Verteilte Systeme - Block 4; Aufgabe 3;
    Gruppe: T18G01
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "memory.h"
#include "hashmap.h"

unsigned int calc_hash(const void *key, const unsigned int key_size, const unsigned int hashmap_size) {
    unsigned int hash = 0;
    unsigned char *position = (unsigned char *) key;

    while(position != ((unsigned char *) key + key_size)) {
        hash = 31 * hash + *position;
        position++;
    }

    return hash % hashmap_size;
}

hash_map create_hashmap(unsigned int size) {
    hash_map new_hashmap;
    new_hashmap.size = size;
    new_hashmap.storage = myalloc(size * sizeof(hash_entry));
    if (new_hashmap.storage == NULL) {
        printf("Can't allocate memory for hashmap!\n");
        exit(1);
    }
    return new_hashmap;
}

hash_entry *new_entry(void *key, const unsigned int key_size, void *value, const unsigned int value_size) {
    hash_entry *new = myalloc(sizeof(hash_entry));
    new->key = key;
    new->value = value;
    new->key_size = key_size;
    new->value_size = value_size;
    new->queue = NULL;
    return new;
}

hash_entry *get(hash_map hashmap, void *key, const unsigned int key_size) {
    unsigned int hash = calc_hash(key, key_size, hashmap.size);
    hash_entry *pos = hashmap.storage[hash];
    while (pos) {
        int diff = 1;
        if (pos->key_size == key_size) {
            diff = memcmp(pos->key, key, key_size);
        }
        if (!diff) {
            if (pos->key != key) free(key);
            printf("HashMap: Get succesful.\n");
            return pos;
        }
        pos = pos->queue;
    }
    free(key);
    return pos;
}

void set(hash_map hashmap, void *key, const unsigned int key_size, void *value, const unsigned int value_size) {
    unsigned int hash = calc_hash(key, key_size, hashmap.size);
    if (!hashmap.storage[hash]) {
        hashmap.storage[hash] = new_entry(key, key_size, value, value_size);
    } else {
        unsigned int key_found = 0;
        hash_entry *pos = hashmap.storage[hash];
        hash_entry *prev = hashmap.storage[hash];
        while (pos) {
            int diff = 1;
            if (pos->key_size == key_size) {
                diff = memcmp(pos->key, key, key_size);
            }
            if (!diff) {
                if (pos->value != value) {
                    free(pos->value);
                    pos->value = value;
                }
                pos->value_size = value_size;

                if (pos->key != key) free(key);
                key_found = 1;
                break;
            }
            prev = pos;
            pos = pos->queue;
        }
        if (!key_found) {
            prev->queue = new_entry(key, key_size, value, value_size);
        }
    }
    printf("HashMap: Set succesful.\n");
    return;
}

int del(hash_map hashmap, void *key, const unsigned int key_size) {
    unsigned int hash = calc_hash(key, key_size, hashmap.size);
    hash_entry *prev = NULL;
    hash_entry *pos = hashmap.storage[hash];
    while (pos) {
        int diff = 1;
        if (pos->key_size == key_size) {
            diff = memcmp(pos->key, key, key_size);
        }
        if (!diff) {
            if (!prev) hashmap.storage[hash] = pos->queue;
                else prev->queue = pos->queue;
            if (pos->key != key) free(key);
            free(pos->key);
            free(pos->value);
            free(pos);
            printf("HashMap: Delete succesful.\n");
            return 1;
        }
        prev = pos;
        pos = pos->queue;
    }
    free(key);
    return 0;
}

void free_hashmap(hash_map hashmap) {
    for (unsigned int i = 0; i < hashmap.size; i++) {
        hash_entry *pos = hashmap.storage[i];
        while (pos) {
            hash_entry *next = pos->queue;
            if (pos->key != NULL) free(pos->key);
            if (pos->value != NULL) free(pos->value);
            free(pos);
            pos = next;
        }
    }
    free(hashmap.storage);
    printf("HashMap: Free done.\n");
    return;
}

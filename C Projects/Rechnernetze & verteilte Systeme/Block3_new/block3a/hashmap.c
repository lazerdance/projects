#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"

unsigned int calc_hash(const void *key, const unsigned int key_length, const unsigned int hashmap_size) {
    unsigned int hash = 0;
    unsigned char *position = (unsigned char *) key;

    while(position != ((unsigned char *) key + key_length)) {
        hash = 31 * hash + *position;
        position++;
    }

    return hash % hashmap_size;
}

hash_map create_hashmap(unsigned int size) {
    hash_map new_hashmap;
    new_hashmap.size = size;
    new_hashmap.storage = calloc(size * sizeof(hash_entry),1);
    if (new_hashmap.storage == NULL) {
        printf("Can't allocate memory for hashmap!\n");
        exit(1);
    }
    return new_hashmap;
}

hash_entry *new_entry(void *key, const unsigned int key_length, void *value, const unsigned int value_length) {
    hash_entry *new = calloc(sizeof(hash_entry),1);
    new->key = key;
    new->value = value;
    new->key_length = key_length;
    new->value_length = value_length;
    new->queue = NULL;
    return new;
}

hash_entry *get(hash_map hashmap, void *key, const unsigned int key_length) {
    unsigned int hash = calc_hash(key, key_length, hashmap.size);
    hash_entry *pos = hashmap.storage[hash];
    while (pos) {
        int diff = 1;
        if (pos->key_length == key_length) {
            diff = memcmp(pos->key, key, key_length);
        }
        if (!diff) {
            if (!memcmp(pos->key, key, key_length)) free(key);
            printf("HashMap: Get succesful.\n");
            return pos;
        }
        pos = pos->queue;
    }
    free(key);
    return pos;
}

void set(hash_map hashmap, void *key, const unsigned int key_length, void *value, const unsigned int value_length) {
    unsigned int hash = calc_hash(key, key_length, hashmap.size);
    if (!hashmap.storage[hash]) {
        hashmap.storage[hash] = new_entry(key, key_length, value, value_length);
    } else {
        unsigned int key_found = 0;
        hash_entry *pos = hashmap.storage[hash];
        hash_entry *prev = hashmap.storage[hash];
        while (pos) {
            int diff = 1;
            if (pos->key_length == key_length) {
                diff = memcmp(pos->key, key, key_length);
            }
            if (!diff) {
                if (pos->value != value) {
                    free(pos->value);
                    pos->value = value;
                }
                pos->value_length = value_length;

                if (pos->key != key) free(key);
                key_found = 1;
                break;
            }
            prev = pos;
            pos = pos->queue;
        }
        if (!key_found) {
            prev->queue = new_entry(key, key_length, value, value_length);
        }
    }
    printf("HashMap: Set succesful.\n");
    return;
}

int del(hash_map hashmap, void *key, const unsigned int key_length) {
    unsigned int hash = calc_hash(key, key_length, hashmap.size);
    hash_entry *prev = NULL;
    hash_entry *pos = hashmap.storage[hash];
    while (pos) {
        int diff = 1;
        if (pos->key_length == key_length) {
            diff = memcmp(pos->key, key, key_length);
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

void cleanHashmap(hash_map hashmap) {
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

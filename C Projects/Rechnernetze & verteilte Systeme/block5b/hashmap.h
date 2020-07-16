/*
    Rechnernetze und Verteilte Systeme - Block 4; Aufgabe 3;
    Gruppe: T18G01
*/

#ifndef hashmap_h
#define hashmap_h

typedef struct hash_entry_struct {
    void *key;
    void *value;
    unsigned int key_size;
    unsigned int value_size;
    struct hash_entry_struct *queue;
} hash_entry;

typedef struct hash_map_struct {
    hash_entry **storage;
    unsigned int size;
} hash_map;

unsigned int calc_hash(const void *key, const unsigned int key_size, const unsigned int hasmap_size);

hash_map create_hashmap(unsigned int size);
hash_entry *new_entry(void *key, const unsigned int key_size, void *value, const unsigned int value_size);

hash_entry *get(hash_map hashmap, void *key, const unsigned int key_size);
void set(hash_map hashmap, void *key, const unsigned int key_size, void *value, const unsigned int value_size);
int del(hash_map hashmap, void *key, const unsigned int key_size);

void free_hashmap(hash_map hashmap);

#endif

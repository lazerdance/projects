/*
    Rechnernetze und Verteilte Systeme - Block 3; Aufgabe 5;
    Gruppe: T18G01
*/

#ifndef hashmap_h
#define hashmap_h

typedef struct hash_entry_struct {
    void *key;
    void *value;
    unsigned int key_length;
    unsigned int value_length;
    struct hash_entry_struct *queue;
} hash_entry;

typedef struct hash_map_struct {
    hash_entry **storage;
    unsigned int size;
} hash_map;

// calculates hashcode given key and hashmap size
unsigned int calc_hash(const void *key, const unsigned int key_length, const unsigned int hasmap_size);

// creates new hashmap
hash_map create_hashmap(unsigned int size);

// creates new entry before inserting in the hashmap
hash_entry *new_entry(void *key, const unsigned int key_length, void *value, const unsigned int value_length);


hash_entry *get(hash_map hashmap, void *key, const unsigned int key_length);

void set(hash_map hashmap, void *key, const unsigned int key_length, void *value, const unsigned int value_length);

int del(hash_map hashmap, void *key, const unsigned int key_length);

void cleanHashmap(hash_map hashmap);

#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashmap.h"

int main() {
    hash_map hashmap = create_hashmap(2);

    char *test = calloc(10, sizeof(char));
    int *a = calloc(1, sizeof(int));
    strncpy(test, "Hallo", 9);
    *a = 1;
    set(hashmap, (void *) test, strlen(test), (void *) a, sizeof(int));
    printf("Adressen: %p\t%p\n", (void *) hashmap.storage[0], (void *) hashmap.storage[1]);

    char *test2 = calloc(10, sizeof(char));
    int *b = calloc(1, sizeof(int));
    strncpy(test2, "HalloAA", 9);
    *b = 2;
    set(hashmap, (void *) test2, strlen(test2), (void *) b, sizeof(int));
    printf("Adressen: %p\t%p\n", (void *) hashmap.storage[0], (void *) hashmap.storage[1]);

    char *test3 = calloc(10, sizeof(char));
    int *c = calloc(1, sizeof(int));
    strncpy(test3, "HalloAAAA", 9);
    *c = 3;
    set(hashmap, (void *) test3, strlen(test3), (void *) c, sizeof(int));
    printf("Adressen: %p\t%p\n", (void *) hashmap.storage[0], (void *) hashmap.storage[1]);

    char *test4 = calloc(10, sizeof(char));
    strncpy(test4, "HalloAA", 9);
    if(!del(hashmap, test4, strlen(test4))) printf("Can't delete.\n");
    printf("Adressen: %p\t%p\n", (void *) hashmap.storage[0], (void *) hashmap.storage[1]);

    free_hashmap(hashmap);
}

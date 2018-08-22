/* === INTROPROG ABGABE ===
 * Blatt 9, Aufgabe 2
 * Tutorium: t05
 * Gruppe: e03
 * Gruppenmitglieder:
 *  - Amro Hendawi
 *  - 
 * ========================
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "introprog_heap.h"


int left(int i) {
    return (2 * i + 1);
}

int right(int i) {
    return (2 * i + 2);
}

int parent(int i) {
    return (i - 1) / 2;
}
/* Reserviere Speicher für einen Heap
 *
 * Der Heap soll dabei Platz für capacity Elemente bieten.
 *
 * Um konsistent mit den Parametern für malloc zu sein, ist capacity
 * als size_t (entspricht unsigned long auf x86/64) deklariert.
 * Da es sauberer ist, sind auch alle Indizes in dieser Aufgabe als
 * size_t deklariert. Ob man size_t oder int als Index für ein Array
 * verwendet, macht in der Praxis (auf x86/64) nur bei Arrays
 * mit mehr als 2.147.483.647 Elementen einen Unterschied.
 */
heap* heap_create(size_t capacity) {
    heap* h = malloc(sizeof (heap));
    h->size = 0;
    h->capacity = capacity;
    h->elements =(int*)calloc(capacity, sizeof(int));
    if(h->elements == NULL) {
        printf("Kein Speicher vorhanden\n");
        return NULL;
    }
    return h;
}


/* Stellt die Heap Bedingung an Index i wieder her
 *
 * Vorraussetzung: Der linke und rechte Unterbaum von i
 * erfüllen die Heap-Bedingung bereits.
 */
void heapify(heap* h, size_t i) {
    size_t links = left(i);
    size_t rechts = right(i);
    size_t largest;

    if(links < h->size && h->elements[links] > h->elements[i]){
            largest = links;
    }
    else {largest = i;
    }
    if(rechts < h->size && h->elements[rechts] > h->elements[largest]) {
        largest = rechts;
    }
    if(largest != i) {
        size_t swap = h->elements[i];
        h->elements[i] = h->elements[largest];
        h->elements[largest] = swap;
        heapify(h, largest);
    }
}
/* Holt einen Wert aus dem Heap
 *
 * Wenn der Heap nicht leer ist, wird die größte Zahl zurückgegeben.
 * Wenn der Heap leer ist, wird -1 zurückgegeben.
 */
int heap_extract_max(heap* h) { 
    if (h->size == 0) {
        return -1;
    }
    int max = h->elements[0];
    h->elements[0] = h->elements[h->size-1];
    h->size = (h->size - 1);
    heapify(h,0);
    return max;
}

/* Fügt einen Wert in den Heap ein
 *
 * Wenn der Heap bereits voll ist, wird -1 zurückgegeben,
 */
int heap_insert(heap* h, int key) {
    int i = h->size;
    if (h->capacity > h->size){
        h->size = h->size + 1;
        i = h->size - 1;
        while (i > 0 && h->elements[parent(i)] < key){
            h->elements[i] = h->elements[parent(i)];
            i = parent(i);
        }
        h->elements[i] = key;
        return 0;
    } else {
            return -1;
    }
}  
/* Gibt den Speicher von einem Heap wieder frei
 */
void heap_free(heap* h){ 
    free(h->elements);
    free(h);
}

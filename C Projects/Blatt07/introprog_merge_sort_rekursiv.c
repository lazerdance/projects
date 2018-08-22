/* === INTROPROG ABGABE ===
 * Blatt 7, Aufgabe 1
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
#include "introprog_input_merge_sort.h"

/*
  Diese Funktion fügt zwei bereits sortierte Arrays
  zu einem sortierten Array zusammen
    array : Pointer auf das Array
    first : Index des ersten Elements (Beginn) des (Teil-)Array
    middle: Index des mittleren Elements des (Teil-)Array
    last  : Index des letzten Elements(Ende) des (Teil-)Array
*/

void merge(int* array, int first, int middle, int last)
{
  int *b = malloc((last - first + 1) * sizeof(int));
  int y = first;
  int m = middle + 1;
  int i = 0;
//  printf("Merge q:%d \t p:%d \t r:%d \t y:%d \t m:%d \t i:%d\n", q, p, r, y ,m ,i);
//  printf("%s\n", "0");
//  print_array(B, (r - p + 1));
  while (y <= middle && m <= last) {
  //  printf("A[y]:%d A[m]:%d\n",A[y],A[m]);
    if (array[y] <= array[m]) {
      b[i++] = array[y++];
  //    printf("%s\n", "1");
  //    print_array(B, (r - p + 1));
    } else {
      b[i++] = array[m++];
  //    printf("%s\n", "2");
  //    print_array(B, (r - p + 1));
    }
  }
  while (y <= middle){
    b[i++] = array[y++];
//    printf("%s\n", "3");
//    print_array(B, (r - p + 1));
  }
  while (m <= last) {
    b[i++] = array[m++];
  }
  int x = 0;
  while (x < i) {
      //printf("Copy: B[x]:%d\t p:%d\t x:%d\n",B[x], p, x);
      array[first + x] = b[x];
      x++;
  }
  //  print_array(B, (r - p + 1));
  free(b);
}
void merge_sort(int* array, int first, int last)
{
  if (first < last) {
    int middle = (first+last)/2;
    merge_sort(array, first, middle);
    merge_sort(array, middle+1, last);
    merge(array, first, middle, last);
  }
}

/*
Hauptprogramm.

Liest Integerwerte aus einer Datei und gibt diese sortiert
im selben Format über die Standardausgabe wieder aus.

Aufruf: ./introprog_merge_sort_rekursiv <maximale anzahl>  <dateipfad>
*/
int main (int argc, char *argv[])
{
    if (argc!=3){
        printf ("usage: %s <maximale anzahl>  <dateipfad>\n", argv[0]);
        exit(2);
    }
    char *filename = argv[2];
    int size = atoi(argv[1]);
    //printf("%d\n", size);
    int * array = malloc(size * sizeof(int));
    int len = read_array_from_file(array, size, filename);
    printf("Eingabe:\n");
    print_array(array, len);
    merge_sort(array, 0, len - 1);
    printf("Sortiert:\n");
    print_array(array, len);
    free(array);
    return 0;
}

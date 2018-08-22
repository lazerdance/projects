/* === INTROPROG ABGABE ===
 * Blatt 1, Aufgabe 1
 * Tutorium: tXX
 * Abgabe von: Erika Mustermann
 * ========================
 */

#include <stdio.h>
#include <stdlib.h>
#include "arrayio.h"

int MAX_LAENGE = 1000;

void insertion_sort(int array[], int len) {
    // HIER insertion sort implementieren
    // Diese Funktion soll das Eingabearray nach dem Insertion Sort-Verfahren
    // sortieren.
    // Hinweis: Verwende die "in place"-Variante! D.h. der Sortiervorgang
    // soll auf dem originalen Array stattfinden und kein zweites verwendet
    // werden.
}

int main(int argc, char *argv[]) {

    if (argc < 2){
        printf("Aufruf: %s <Dateiname>\n", argv[0]);
        printf("Beispiel: %s zahlen.txt\n", argv[0]);
        exit(1);
    }

    char *filename = argv[1];

    int array[MAX_LAENGE];
    int len = read_array_from_file(array, MAX_LAENGE, filename);

    printf("Unsortiertes Array:");
    print_array(array, len);

    // Aufruf insertion sort

    printf("Sortiertes Array:");
    print_array(array, len);

    return 0;
}

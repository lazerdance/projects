/* === INTROPROG ABGABE ===
 * Blatt 1, Aufgabe 2
 * Tutorium: tXX
 * Abgabe von: Erika Mustermann
 * ========================
 */

#include <stdio.h>
#include <stdlib.h>
#include "arrayio.h"

int MAX_LAENGE = 1000;
int MAX_VALUE = 100;

void count_sort_calculate_counts(int input_array[], int len, int count_array[]) {
    // Hier Funktion implementieren
}

void count_sort_write_output_array(int output_array[], int len, int count_array[]) {
    // Hier Funktion implementieren
}



int main(int argc, char *argv[]) {

    if (argc < 2){
        printf("Aufruf: %s <Dateiname>\n", argv[0]);
        printf("Beispiel: %s zahlen.txt\n", argv[0]);
        exit(1);
    }

    char *filename = argv[1];

    int input_array[MAX_LAENGE];
    int len = read_array_from_file(input_array, MAX_LAENGE, filename);

    printf("Unsortiertes Array:");
    print_array(input_array, len);

    // HIER alle nötigen Deklarationen und Funktionsaufrufe für Count Sort einfügen

    printf("Sortiertes Array:");
    // Folgende Zeile einkommentieren, um das Array auszugeben
    // print_array(output_array, len);

    return 0;
}

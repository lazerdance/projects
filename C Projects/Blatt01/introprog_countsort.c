/* === INTROPROG ABGABE ===
 * Blatt 1, Aufgabe 2
 * Tutorium: t05
 * Abgabe von: Amro Hendawi
 * ========================
 */

#include <stdio.h>
#include <stdlib.h>
#include "arrayio.h"

int MAX_LAENGE = 1000;
int MAX_VALUE = 100;
int i,j,k;
void count_sort_calculate_counts(int input_array[], int len, int count_array[]) {
    // Hier Funktion implementieren
    for (j=0; j<MAX_LAENGE;j++){
        count_array[j] = 0;
    }
    for (i=0; i<len;i++){
        count_array[input_array[i]] = count_array[input_array[i]] + 1;
    }
}

void count_sort_write_output_array(int output_array[], int len, int count_array[]) {
    // Hier Funktion implementieren
	k = 0;
	for ( j=0; j<MAX_VALUE+1; j++){
    for (i=0 ; i<count_array[j]; i++){
            output_array[k] = j;
            k=k+1;
        }
  }
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
	int count_array[MAX_LAENGE];
	int output_array[MAX_LAENGE];
    count_sort_calculate_counts(input_array, len, count_array);
    count_sort_write_output_array(output_array, len, count_array);
    printf("Sortiertes Array:");
    // Folgende Zeile einkommentieren, um das Array auszugeben
    // print_array(output_array, len);
	    print_array(output_array, len);
    return 0;
}

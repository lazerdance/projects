/* === INTROPROG ABGABE ===
 * Blatt 4, Aufgabe 2
 * Tutorium: t05
 * Abgabe von: Amro Hendawi
 * ========================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "introprog_structs_lists_input.h"

#define MAX_STR 255

/* Bewirkt, dass statt 'struct _element' auch 'element' verwendet werden kann. */
typedef struct _element element; 

/* Bewirkt, dass statt 'struct _list' auch 'list' verwendet werden kann. 
 * Hier in einem geschrieben, so dass man auch 'list' innerhalb der 
 * struct-Definition selbst verwenden kann.  
 */
typedef struct _list { /* Separater Wurzelknoten */
    element *first;    /* Anfang/Kopf der Liste */
    int count;         /* Anzahl der Elemente */
} list;

/* HIER struct _element implementieren. */

struct _element{
    char title[MAX_STR];
    char author[MAX_STR];
    int year;
    long long int isbn;
    element *next;
};
/* Fuege ein Element in die Liste ein, sodass die Liste aufsteigend nach ISBN
 * sortiert ist.
 * Dafür muss das erste Element ermittelt werden, dass in der
 * bisher sortierten Liste eine ISBN besitzt die größer ist als die des neuen
 * Elements. Wenn die Liste leer ist soll das Element direkt an den Anfang
 * platziert werden.
 *
 * first      - Erstes Element (bzw. Anfang) der Liste
 * new_elem   - Neues Element das in die Liste eingefuegt werden soll.
 *
 * Gib einen Pointer auf den neuen oder alten Anfang der Liste zurueck.
 */
element* insert_sorted(element *first, element *new_elem) {
    /* HIER implementieren. */
        if (first == NULL) {
		return new_elem;}
    element *tmp;
        if (first->isbn >=new_elem->isbn){
	new_elem->next=first;
	first=new_elem;
        }else{	tmp=first;
	while(tmp->next!=NULL && tmp->next->isbn<new_elem->isbn){
	tmp=tmp->next;
    }
		new_elem->next=tmp->next;
		tmp->next=new_elem;
	}
	
		return first;
}
/* Kreiere ein neues Element mit dynamischem Speicher.
 *
 * title     - Der Titel des Buches
 * author    - Autor des Buches
 * year      - Erscheinungsjahr des Buches
 * isbn      - ISBN des Buches
 *
 * Gib einen Pointer auf das neue Element zurueck.
 */
element *construct_element(char *title, char* author, int year, long long int isbn) {
    /* HIER implementieren. */
   element *elem = malloc(sizeof(element));
    strncpy(elem->title,title, MAX_STR-1);
    strncpy(elem->author,author, MAX_STR-1);
	elem->title[MAX_STR-1] = '\0';
	elem->author[MAX_STR-1] = '\0';
    elem->year = year;
    elem->isbn = isbn;

  elem->next = NULL;
    return elem;
}

/* Gib den der Liste und all ihrer Elemente zugewiesenen Speicher frei. */
void free_list(list *alist) {
    /* HIER implementieren. */
    element *elem = alist -> first;
    element *alt;
    while (elem != NULL) {
        alt = elem;
        elem = elem -> next;
        free(alt);
    }
    free(elem);
    free(alist);
}

/* Lese die Datei ein und fuege neue Elemente in die Liste ein 
 * _Soll nicht angepasst werden_
 * */
void read_list(char* filename, list *alist) {
    element* new_elem;
    char* title;
    char* author;
    int year;
    long long int isbn;
    read_line_context ctx;
    open_file(&ctx, filename);
    while(read_line(&ctx, &title, &author, &year, &isbn) == 0) {
        new_elem = construct_element(title, author, year, isbn);
        alist->first = insert_sorted(alist->first, new_elem);
        alist->count++;
    }
}

/* Erstelle die Liste:
 *  - Weise ihr dynamischen Speicher zu 
 *  - Initialisiere die enthaltenen Variablen
 * _Soll nicht angepasst werden_
 */
list* construct_list() {
    list *alist = malloc(sizeof(list));
    alist->first = NULL;
    alist->count = 0;
    return alist;
}

/* Gib die Liste aus:
 * _Soll nicht angepasst werden_
 */
void print_list(list *alist) {
    printf("Meine Bibliothek\n================\n\n");
    int counter = 1;
    element *elem = alist->first;
    while (elem != NULL) {
        printf("Buch %d\n", counter);
        printf("\tTitel: %s\n", elem->title);
        printf("\tAutor: %s\n", elem->author);
        printf("\tJahr:  %d\n", elem->year);
        printf("\tISBN:  %lld\n", elem->isbn);
        elem = elem->next;
        counter++;
    }
}

/* Main Funktion
 * _Soll nicht angepasst werden_
 */
int main(int argc, char** argv) {
    list *alist = construct_list();
    read_list(argc>1?argv[1]:"buecherliste.txt", alist);
    print_list(alist);
    free_list(alist);
    return 0;
}

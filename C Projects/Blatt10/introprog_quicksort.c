/* === INTROPROG ABGABE ===
 * Blatt 10, Aufgabe 1
 * Tutorium: t05
 * Gruppe: e03
 * Gruppenmitglieder:
 *  - Amro Hendawi
 *  - 
 * ========================
 */

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "quicksort.h"

/*****************************************************
 * Die benoetigten structs findet Ihr in quicksort.h *
 *****************************************************/

void init_list(list* mylist)
{
// HIER Liste initialisieren
    mylist->first=NULL;
	mylist->last=NULL;
}



// Diese Funktion fügt Listenelemente an die Liste an
void insert_list(list_element* le, list* mylist)
{
    // HIER Code einfügen
		if (mylist->first == NULL) {
		mylist->first = le;
		mylist->last = le;
		mylist->last->next = NULL;
	}
	else {
		mylist->last->next = le;
		mylist->last = le;
		mylist->last->next = NULL;
    }
}

// Speicher für Listenelemente wieder freigeben
void free_list(list* mylist)
{
    // HIER Code einfügen
	list_element *current = mylist->first;
    list_element *alt=NULL;
	while (current) {
        alt = current;
		current = current->next;
        free(alt->password);
        free (alt);
    }
}


int words(char *string) {
	int counter = 0;
	int len = strlen(string);
	char lastC;
	
	if(len > 0) {
		lastC = string[0];
	}
	for(int i = 0; i <= len; i++) {
		if((string[i] == ' ' || string[i] == '\0') && lastC != ' ') {
			counter++;
		}
		lastC = string[i];
	}
	return counter;
}

// Namen, Zahlen Paare in Liste einlesen
 void read_data(char* filename, list* mylist)
{

	FILE *fp = fopen(filename, "r");
	if(fp == NULL) {
		perror("Error");
		return;
	}
	char buffer[100];
	char pw[100];
	char *strbuffer;
	char pwcountbuffer[100];
	int pwcount;
	int pwwords;
	memset(pw, 0, sizeof(pw));

	while(fgets(buffer, sizeof(buffer), fp) != NULL) {
		pwwords = words(buffer) - 1;
		strbuffer = strtok(buffer, " ");
		for(int i = 0; i < pwwords; i++) {
			strcat(pw, strbuffer);
			if(pwwords > 1 && i != pwwords - 1) {
				strcat(pw, " ");
			}
			strbuffer = strtok(NULL, " ");
		}
		strcpy(pwcountbuffer, strbuffer);
		pwcount = atoi(pwcountbuffer);
	
		list_element *newle = (list_element*)malloc(sizeof(list_element));
		if(newle == NULL) {
			perror("Overflow!");
			return;
		}
	
		newle -> password = (char*)malloc(sizeof(pw) * sizeof(char));
		strcpy(newle -> password, pw);
		newle -> count = pwcount;
		insert_list(newle, mylist);
		memset(pw, 0, sizeof(pw));
	}
	fclose(fp);
}

// Liste teilen. Teillisten werden in left und right zurück gegeben

list_element* partition( list* input, list* left, list* right )
{
	list_element *pivot;

	if(input -> first == NULL) {
		pivot = NULL;
		input -> last = NULL;
	}
	else {
		pivot = input -> first;
		input -> first = pivot -> next;
		pivot -> next = NULL;
	}

	if(pivot != NULL) {
		list_element *temp = input -> first;
		while(temp != NULL) {
			input -> first = temp -> next;
			if(input -> first == NULL) {
				input -> last = NULL;
			}
				if(temp -> count >= pivot -> count) {
					insert_list(temp, right);
				}
				else {
					insert_list(temp, left);
				}
			temp = input -> first;
		}
	}
	return pivot;
}

// Hauptfunktion des quicksort Algorithmus
void qsort_list(list* mylist)
{
	if(mylist -> first == mylist -> last) {
		return;
	}
	else {
		list lstack;
		list rstack;
		list *left = &lstack;
		list *right = &rstack;
		init_list(left);
		init_list(right);

		list_element *pivot = partition(mylist, left, right);
		qsort_list(left);
		qsort_list(right);

		if(left -> first == NULL) {
			mylist -> first = pivot;
		}
		else {
			mylist -> first = left -> first;
			left -> last -> next = pivot;
		}

		if(right -> first == NULL) {
			pivot -> next = NULL;
			mylist -> last = pivot;
		}
		else {
			pivot -> next = right -> first;
			mylist -> last = right -> last;
		}
	}
}

// Liste ausgeben
void print_list(list* mylist)
{
    // HIER Code einfügen:
    // * Laufe über die list_element in mylist und gebe sie aus.
	if(mylist->first == NULL) {
		printf("List is empty!\n\n");
		return;
	}
    list_element *elem = mylist->first;
    while (elem) {
        printf("%s %d\n", elem->password, elem->count);
        elem = elem->next;
	}
}

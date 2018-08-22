/* === INTROPROG ABGABE ===
 * Blatt 6, Aufgabe 2
 * Tutorium: t05
 * Gruppe: e03
 * Gruppenmitglieder:
 *  - Amro Hendawi
 * ========================
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>   //definiert den speziellen Wert NaN für floats
#include "introprog_input_stacks-rpn.h"

typedef struct _stack stack;
typedef struct _stack_element stack_element;

struct _stack {
    stack_element* top;
};

struct _stack_element {
    stack_element* next;
    float value;
};

void stack_push(stack* astack, float value)
{
    /* HIER implementieren */

    stack_element *elem = (stack_element*)malloc(sizeof(stack_element));
    elem->value = value;
    elem->next = astack->top;
    astack->top = elem;
}

float stack_pop(stack* astack)
{
    /* HIER implementieren */
	float r_value;
if(astack != NULL){
  stack_element *temporary = astack->top;
     r_value = temporary->value;
    astack->top = temporary->next;
    free(temporary);
}
else { if(isnan(astack->top->value) == 1){
	r_value=0;	}
}
    return r_value;
}

void process(stack* astack, char* token)
{
    /* HIER implementieren */
    if(is_number(token)){
        float number = atof(token);
        stack_push(astack, number);
    }
   else if(is_add(token)){
        float number1 = stack_pop(astack);
	float number2 = stack_pop(astack);
        float result = number1 + number2;
        stack_push (astack, result);
    }
    else if(is_mult(token)){
        float number1 = stack_pop(astack);
	float number2  = stack_pop(astack);
        float prdkt = number1 * number2;
        stack_push(astack, prdkt);
    }
    else if(is_sub(token)){
        float number1 = stack_pop(astack);
	float number2  = stack_pop(astack);
        float diff = number2 - number1;
        stack_push(astack, diff);
    }
    printf("\n<Logik fehlt!>\n");
    return;
}


/* Debugausgabe des Stack
 * Diese Funktion kannst du zum debugging des Stack verwenden.
 *
 * astack    - Ein Pointer auf den Stack
 */
void print_stack(stack *astack) {
    int counter = 0;
    printf("\n |xxxxx|xxxxxxxxxxxxxxxxxxx|xxxxxxxxxxxxxxxxxxx|xxxxxxxxx|\n");
    printf(" | Nr. | Adresse           | Next              | Wert    |\n");
    printf(" |-----|-------------------|-------------------|---------|\n");
    for (stack_element* elem=astack->top; elem != NULL; elem = elem->next) {
        printf(" | %3d | %17p | %17p | %7.3f |\n", counter, elem, elem->next, elem->value);
        counter++;
    }
    printf(" |xxxxx|xxxxxxxxxxxxxxxxxxx|xxxxxxxxxxxxxxxxxxx|xxxxxxxxx|\n");
}

/* Kreiere einen Stack mit dynamischen Speicher.
 * Initialisiere die enthaltenen Variablen.
 *
 * Gebe einen Pointer auf den Stack zurück.
 */
stack* stack_erstellen() {
    /* HIER implementieren */
    stack *nastack = malloc(sizeof(stack));
    nastack->top = NULL;
    return nastack;
}

int main(int argc, char** args)
{
    stack* astack = stack_erstellen();
    char zeile[MAX_STR];
    char* token;

    intro();
    while (taschenrechner_input(zeile) == 0) {
        // Erstes Token einlesen
        token = strtok(zeile, " ");

        while (token != NULL) {
            printf("Token: %s\n", token);
            // Stackoperationen durchführen
            process(astack, token);
            // Nächstes Token einlesen
            token = strtok(NULL, " ");
            print_stack(astack);
        }

        printf("\nExtrahiere Resultat\n");
        float result = stack_pop(astack);
        print_stack(astack);

        if (astack->top != NULL) {
            while (astack->top != NULL) {
                stack_pop(astack);   //Räume Stack auf
            }
            printf("\nDoes not Compute: Stack nicht leer!\n");
        } else if (result != result) {
            printf("\nDoes not Compute: Berechnung fehlgeschlagen!\n");
        } else {
            printf("\nDein Ergebnis:\t%7.3f\n\n", result);
        }
    }
    free(astack);
}

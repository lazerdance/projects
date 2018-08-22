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

#include "introprog_telefonbuch.h"
// Fügt einen Knoten mit der Telefonnummer phone und dem Namen name in den
// Binären Suchbaum bst ein. Für den Suchbaum soll die Eigenschaft gelten, dass
// alle linken Kinder einen Wert kleiner gleich (<=) und alle rechten Kinder
// einen Wert größer (>) haben.
void bst_insert_node(bstree* bst, unsigned long phone, char *name)
{
    int size=strlen(name)+1;
    bst_node* test;
    test=find_node(bst,phone);
    if(test)
    {
        printf ("Fehler: Ungültige Telefonnummer\n");
        return;
    }
    char *new_name = malloc (sizeof(char) *size);
    // Kreiere Pointer für neuen Namen und alloziere statischen Speicher MAX_STR = 255
    snprintf(new_name, size, "%s", name);
    // Speichere übergeben Namen in new_name
    bst_node *current = bst->root;
    // Kreiere temporäre Pointer für root und parent
    bst_node *current_parent;
    bst_node *new_node = malloc (sizeof(bst_node));
    // Kreieren eines neuen Knoten-Pointer und allozieren von dynamischen Speicher dafür
    if (bst->root == NULL)
    {
        // Ist die Wurzel leer so wird neuer Knoten zur Wurzel
        bst->root = new_node;
        new_node->left = NULL;
        new_node->right = NULL;
        new_node->parent = NULL;
        new_node->phone = phone;
        new_node->name = new_name;
    }
    else
    {
        while (current != NULL)
        {
            // Durchsuchen des BST ob die Nummer vorhanden ist
            if (current->phone == phone )
            {
                // Vergleiche ob neu Nummer vorhanden ist und gib Speicher wieder frei
                printf("Fehler: Ein Knoten mit dieser Nummer besteht schon\n");
                free (new_name);
                free (new_node);
                return;
            }
            else if (phone <= current->phone)
            {
                // Ist einzufügende Nummer kleiner als Wurzel-Nummer? Gehe den Baum links lang
                current_parent = current;
                // Sichern des Wurzelknoten im temporären Elternknoten
                current = current->left;
                // Linkes Kindknoten wird zum Wurzelknoten
            }
            else {  // Sonst gehe den Baum rechts lang
                current_parent = current;  // Sichern des Wurzelknoten im temporären Elternknoten
                current = current->right;  // Rechtes Kindknoten wird zum Wurzelknoten
            }
        }  // Einzufügende Stelle des entsprechenden Elternknoten ist gefunden, dank der While-Schleife
        if (phone <= current_parent->phone){  // Packe neue Nummer an den leeren linken Kindknoten, wenn neue Nummer <= Elternnummer
            current_parent->left = new_node;
            new_node->left = NULL;
            new_node->right = NULL;
            new_node->parent = current_parent;
            new_node->phone = phone;
            new_node->name = new_name;
        }

        else {      // Packe neue Nummer an den leeren rechten Kindknoten
            current_parent->right = new_node;
            new_node->left = NULL;
            new_node->right = NULL;
            new_node->parent = current_parent;
            new_node->phone = phone;
            new_node->name = new_name;
        }
    }
}

// Diese Funktion liefert einen Zeiger auf einen Knoten im Baum
// mit dem Wert phone zurück, falls dieser existiert. Ansonsten wird
// NULL zurückgegeben.
bst_node* find_node(bstree* bst, unsigned long phone)
{
    if (phone < 1 && phone > 10000){
        // Telefonnummer ist zwischen 1-9999
        printf ("Fehler: Ungültige Telefonnummer\n");
        return NULL;
    }
    bst_node *current = bst->root;
    // Quasi das selbe Spiel wie bei bst_insert_node
    while (current != NULL )
    {
        if (phone == current->phone)
        {
            return current;
        }
        else if (phone <= current->phone)
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }
    }
    return NULL;
}


// Forward-declaration von der Funktion bst_in_order_walk_node
// Gibt den Unterbaum von node in "in-order" Reihenfolge aus
void bst_in_order_walk_node(bst_node* node)
{
    // Linkes Kind <= Rechtes Kind
    if (node != NULL) {
        // Gehe den Baum zuerst links ab
        bst_in_order_walk_node(node->left);
        // Rekursiveraufruf des linken Kindes
        print_node(node);
        // Ausgabe des Knoten
        bst_in_order_walk_node(node->right);
        // Rekursiveraufruf des rechten Kindes
    }
}

// Gibt den gesamten Baum bst in "in-order" Reihenfolge aus. Die Ausgabe
// dieser Funktion muss aufsteigend soriert sein.
void bst_in_order_walk(bstree* bst)
{
    if (bst != NULL) {
        bst_in_order_walk_node(bst->root);
    }
}

// Löscht den Teilbaum unterhalb des Knotens node rekursiv durch
// "post-order" Traversierung, d.h. zurerst wird der linke und dann
// der rechte Teilbaum gelöscht. Anschließend wird der übergebene Knoten
// gelöscht.
void bst_free_subtree(bst_node* node)
{
    if (node != NULL)
    {
        // Löschen des Knoten, wenn kein Kind vorhanden
        bst_free_subtree(node->left);
        // Rekursiveraufruf des linken Kindes
        bst_free_subtree(node->right);
        // Rekursiveraufruf des rechten Kindes
        free(node->name);
        // Freigabe des allozierten Speicher für den Namen
        free(node);
        // Freigabe des Knoten
    }
}

// Löscht den gesamten Baum bst und gibt den entsprechenden Speicher frei.
void bst_free_tree(bstree* bst)
{
    if(bst != NULL && bst->root != NULL)
    {
        bst_free_subtree(bst->root);
        bst->root = NULL;
    }
}

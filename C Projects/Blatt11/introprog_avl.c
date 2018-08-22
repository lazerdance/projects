/* === INTROPROG ABGABE ===
 * Blatt 11, Aufgabe 2
 * Tutorium: t05
 * Gruppe: e03
 * Gruppenmitglieder:
 *  - Amro Hendawi
 *  - 
 * ========================
 */

#include <stdlib.h>
#include <stdio.h>    //Ein- / Ausgabe
#include <math.h>     //Für die Berechnungen der Ausgabe
#include "avl.h"

int max(int a, int b){
	if(a>b){
		return a;
	}
	else{
		return b;
	}
}

int height (struct AVLNode* node)
{
    if (node== NULL){
    	return 0;
    }
    return node->height;
}


void height_node (struct AVLNode* node) 
{
     		node->height= 1+max(height(node->left),height(node->right));	
}

// Gibt den gesamten AVL Baum in "in-order" Reihenfolge aus.
void AVL_node_order(AVLNode* node) 
{
	if (node != NULL) {   
    	AVL_node_order(node->left); //move to left
    	printf ("%d ", node->value);
    	AVL_node_order (node->right); //move to right
	}
} 

void AVL_in_order_walk(AVLTree* avlt)
{
    if (avlt->root != NULL) {
    	AVL_node_order(avlt->root);
    }
    printf("\n");
}

// Diese Funktion führt eine Linksrotation auf dem angegebenen Knoten aus.
void AVL_rotate_left(AVLTree* avlt, AVLNode* x)
{
    AVLNode* y;
	y= x->right;
    x->right = y->left;
	if (y->left != NULL) {
		y->left->parent=x;
	}
	y->parent = x->parent;
	if(x->parent==NULL){
		avlt->root = y;
	}
	else if(x==x->parent->left){
		x->parent->left=y;	
	}
	else{
		x->parent->right=y;
	}
	y->left=x;
	x->parent=y;
	height_node (x);
 	height_node (y);
}
// Diese Funktion führt eine Rechtsrotation auf dem angegebenen Knoten aus.
// Beachtet: Die Höhen der entsprechenden Teilbäume müssen (lokal) 
// angepasst werden.
void AVL_rotate_right(AVLTree* avlt, AVLNode* y)
{
    //Hier Code implementieren!
    AVLNode* x;
    x = y->left;
    y->left = x->right;
 	if (x->right != NULL) {
 		x->right->parent = y;
 	}
 	x->parent = y->parent;
 	if (y->parent == NULL) {
 		avlt->root = x;
 	}
 	else if (y == y->parent->right) {
 		y->parent->right = x;
 	}
 	else {
 		y->parent->left = x;
 	}
 	 x->right = y;
    y->parent = x;
    height_node (y);
    height_node (x); 
}

//Balanciere den Teilbaum unterhalb von node.
void AVL_balance(AVLTree* avlt, AVLNode* node)
{
	AVLNode *lc = node->left;
	AVLNode *rc = node->right;
    if (height(lc)> height(rc)+1) {
    	if(height(lc->left) < height(lc->right)) { 
    		AVL_rotate_left(avlt, lc); //Linksrotation(lc[y])
    	}
    	AVL_rotate_right(avlt, node); //Rechtsrotation(y)
    }

    else if (height(rc ) > height(lc )+1) {
    	if ( height (rc->right) < height (rc->left)) {
    		AVL_rotate_right(avlt, rc); //Rechtsrotation(rc[y])
    	}
     AVL_rotate_left(avlt, node); //Linksrotation(y)
 	}
}


void AVL_INSERT (AVLTree* avlt, AVLNode* node, int value) 
{
	  if (value < node->value) { //else if key[x]<key[t]
			if (node->left != NULL) {
    			AVL_INSERT(avlt, node->left,value); //then AVL-Einfügen(lc[t],x
    		} 
    		else {
    			 AVLNode* t=(AVLNode*)calloc(1,sizeof(AVLNode));
    				t->parent=node;
    				t->value=value;
    				t->height=1;
    				node->left=t;
    		}
    }
	
    else if ( value > node->value) { //else if key[x]>key[t] 
		if (node->right != NULL) {
    	AVL_INSERT(avlt, node->right,value);
    	}
    	else {
    			 AVLNode* t=(AVLNode*)calloc(1,sizeof(AVLNode));
    				t->parent=node;
    				t->value=value;
    				t->height=1;
    				node->right=t;
    	} 
    }
    else { 
    	return;
    }
    if (node->left == NULL){
   	node->height= 1+max(0,node->right->height);
    }
    else if (node->right== NULL){
     	node->height= 1+max(node->left->height,0);
    }
    else {
     		node->height= 1+max(node->left->height,node->right->height);
	} 
    	AVL_balance(avlt,node);
}


// Fügt der Wert value in den Baum ein.
// Die Implementierung muss sicherstellen, dass der Baum nach dem Einfügen
// immer noch balanciert ist!
void AVL_insert_value(AVLTree* avlt, int value)
{   
    if (avlt->root == NULL) { //if t=nil then
    	AVLNode* knoten=(AVLNode*)calloc(1,sizeof(AVLNode));
    	avlt->root=knoten;
    	avlt->root->value = value;
    	avlt->root->height = 1;
    	avlt->root->left = NULL;
    	avlt->root->right = NULL;
    	avlt->root->parent = NULL;
    }
    else {
    		AVL_INSERT (avlt,avlt->root,value);
    }
	avlt->numberOfNodes=avlt->numberOfNodes+1;
    return;
}

void avl_free(AVLNode* node) {
	if (node != NULL) {	
		avl_free(node->left); 
		avl_free(node->right);
		free(node);
	}
}
// Löscht den gesamten AVL-Baum und gibt den Speicher aller Knoten frei.
void AVL_remove_all_elements(AVLTree* avlt)
{
  if(avlt != NULL && avlt->root != NULL) 
  {
        avl_free(avlt->root);
        avlt->root = NULL;
	}
}
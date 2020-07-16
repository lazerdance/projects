#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef QUEUE_H
#define QUEUE_H

//struct that represents a node inside a queue
typedef struct node {
	struct node* next;
	struct node* prev;
	void* data;
	int size;
} node;

//struct representing the queue structure itself
typedef struct queue {
	node* head;
	node* tail;
	int size;
} queue;


/*
	creates, initializes and returns a new node
*/
node* create_node(void* data, unsigned int size);


/*
	frees the given node. DOES NOT FREE THE NODES DATA!
*/
void free_node(node* n);


/*
	frees the given queue and all its nodes
*/
void free_Q(queue* q);


/*
	initializes the given queue
*/
queue* init_Q(queue* q);


/*
	appends the given node to the specified queue
*/
void append_node(node* n, queue* q);


/*
	removes the given node from the specified queue
*/
node* remove_node(node* to_remove, queue* q);



#endif
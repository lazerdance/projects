#include <string.h>
#include <stdio.h>
#include "../matrix.h"
#include "queue.h"

#ifndef PARSER_H
#define PARSER_H

//struct to hold all the data extracted from a file
typedef struct state {
	int m;				//number of processes
	int n;				//number of (different) resources
	int* f; 			//array for available resources
	queue a_array;		//queue-struct for storing pending allocations
	queue* allocations;	//just a pointer to that queue-struct
} state;


//struct to hold information about an allocation
typedef struct alloc_obj {
	matrix* row;		//1xn matrix containing the desired resources
	int n;				//number of (different) resources
	int ID;				//ID of the requesting process
} alloc_obj;


/*
	allocates and returns a new struct of type state
*/
state* create_state(int m, int n);


/*
	frees the given struct and its data
*/
void free_state(state* s);


/*
	allocates and returns a new struct of type alloc_obj
*/
alloc_obj* create_alloc_obj(matrix* row, int ID);


/*
	frees the given struct and its data
*/
void free_alloc_obj(alloc_obj* a);


/*
	converts a char array of numbers into the corresponding int array
*/
int fill_array(char* to_read, int* to_fill, int num);


/*
	reads the given file and returns its data as a struct of type state
*/
state* read_data(char* name);


/*
	dequeues the given node from the specified queue. Only to be used for removing "allocation"-nodes from their queues 
*/
void dequeue_allocation(queue* q, node* allocation);


/*
	removes the given element from the specified queue and appends it afterwards
*/
void move_allocation_to_back(queue* q, node* allocation);


/*
	prints the given array
*/
void print_array(int* a, int len);


/*
	prints an allocation request
*/
void print_alloc(alloc_obj* a, int alloc_num);


/*
	prints the data of the given struct of type state
*/
void print_state(state* s);
#endif
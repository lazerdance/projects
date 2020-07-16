#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef MATRIX_H
#define MATRIX_H

typedef struct matrix {
	unsigned int m;		//"height", number of rows/processes
	unsigned int n;		//"width",  number of columns/resources

	int* elements;		//array that holds all the elements
} matrix;


/*
	creates a new matrix (allocates memory!) and initializes its parameters

	unsigned int m: 	number of rows
	unsigned int n: 	number of columns
	int* numbers:		int array containing the values to be represented by the matrix.

												 	 ----- 	
													| 0 1 |
	exaple: create_matrix(3, 2, [0,1,2,3,4,5]) -->	| 2 3 |
													| 4 5 |
													 -----

	returns: a pointer to the new matrix
*/
matrix* create_matrix(unsigned int m, unsigned int n, int* numbers);


/*
	creates a new matrix around a given row. All other rows are meant to be zero

	unsigned int m:			total number of rows the new matrix should have
	unsigned int row_nr:	index of the row that needs to be overwritten by the given row
	matrix* row:			a matrix representing the row to be duplicated (the user is responsible for making sure that
							this matrix actualy has exactly one row)
														 ------- 	
														| 0 0 0 |
	exaple: create_matrix_from_row(3, 2, [1,2,3]) -->	| 0 0 0 |
														| 1 2 3 |
														 -------
	returns: a pointer to the new matrix
*/
matrix* create_matrix_from_row(unsigned int m, unsigned int row_nr, matrix* row);


/*
	frees all the memory that has been dynamically allocated during creation of the given matrix

	matrix* matrix: matrix to free
*/
void free_matrix(matrix* matrix);


/*
	duplicates the given matrix (needs to allocate new memory)

	matrix* old: matrix to duplicate

	returns: a pointer to the freshly created matrix
*/
matrix* duplicate_matrix(matrix* old);


/*
	element-wise addition of the given matrices (does NOT create a new matrix).
	After the operation, matrix a holds the result of the addition while matrix b remains unchanged.

	matrix* a: ...
	matrix* b: ...
*/
void add_matrix(matrix* a, matrix* b);


/*
	element-wise subtraction of the given matrices (subtracts b of a (a-b), does NOT create a new matrix).
	After the operation, matrix a holds the result of the subtraction while matrix b remains unchanged.

	matrix* a: ...
	matrix* b: ...
*/
void subtract_matrix(matrix* a, matrix* b);


/*
	returns the element at the indices i,j (IMPORTANT: (i,j)=(1,3) means that the 4th element 
		in row 2 is wanted as indexing starts at 0)

	matrix* matrix: matrix to extract the element from
	unsigned int i:	row index
	unsigned int j:	column index

	if parameters are out auf range, returns -1

*/
int get_elem_of_matrix(matrix* matrix, unsigned int i, unsigned int j);


/*
	checks all of the matrix' elements for being greater, equal to or less than zero 

	matrix* matrix: matrix to check

	returns: 	-1 if any element is <0
				0 if all elements are =0
			 	1 if all elements are >=0
*/
int check_matrix_elements(matrix* matrix);


/*
	extracts one row from the given matrix. The row itself again is representated by another matrix. Remember that
		indexing starts at 0

	matrix* matrix: matrix to extract the row from
	unsigned int m:	index of the row to be extracted

	returns: a pointer to a new matrix in the shape of [1 x n] (1 row, n columns) representating the desired row
*/
matrix* get_row_from_matrix(matrix* matrix, unsigned int m);

/*
	prints the given matrix to the console

	matrix* matrix: matrix to print
*/
void print_matrix(matrix* matrix);

#endif
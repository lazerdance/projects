#include "matrix.h"

matrix* create_matrix(unsigned int m, unsigned int n, int* numbers)
{
	/*TODO:
			-Argumente auf Gueltigkeit ueberpruefen
			-neue Matrix erstellen und zurueckgeben (Pointer!!!)
			-(das Array soll dabei zwar kopiert, aber ansonsten UNVERAENDERT abgespeichert werden.
			  Dies ist für die automatischen Tests unerlaesslich)
	*/
	if(m <= 0 || n <= 0){
		printf("one of the dimensions is incorrect\n");
		return NULL;
	}
	if(numbers == NULL){
		printf("elements array is invalid\n");
		return NULL;
	}
	// int arr_length = sizeof(numbers)/sizeof(int);
	// if(n*m != arr_length){
	// 	printf("elements array should have number of elements equal to the matrix fields %d\n\n",arr_length);
	// }

	matrix * mx = malloc(sizeof(matrix));

	mx->m = m;
	mx->n = n;
	mx->elements = malloc(n*m*sizeof(int));
	for(int i=0; i<n*m; i++){
		mx->elements[i] = numbers[i];
	}

	return mx;
}

matrix* create_matrix_from_row(unsigned int m, unsigned int row_nr, matrix* row)
{
	/*TODO
			-Argumente auf Gueltigkeit ueberpruefen
			-Erstellen einer neuen Matrix (Initialisierung mit 0)
			-Kopieren der gegebenen Zeile an die richtige Stelle
	*/
	
	if(m <= 0 || row_nr < 0 || row_nr >= m){
		printf("the given dimension should withing the matrix \n");
		return NULL;
	}
	if(row == NULL){
		printf("the given row matrix is uninitialized\n");
		return NULL;
	}

	matrix * mx = malloc(sizeof(matrix));
	mx->m = m;
	mx->n = row->n;
	mx->elements = malloc((m*mx->n)*sizeof(int));
	for(int i=0; i<m*mx->n; i++){
		mx->elements[i] = 0;
	}
	for(int i=0; i<mx->n; i++){
		mx->elements[i+mx->n*row_nr] = row->elements[i];
	}

	return mx;

}

void free_matrix(matrix* matrix)
{
	/*TODO:
			-Argumente auf Gueltigkeit ueberpruefen
			-Matrix und alle dynamisch reservierten elemente der Matrix freigeben
	*/
	if(matrix == NULL) return;
	free(matrix->elements);
	free(matrix);
}

matrix* duplicate_matrix(matrix* old)
{
	/*TODO:
			-Argumente auf Gueltigkeit ueberpruefen
			-Erstellen und Zurückgeben einer NEUEN Matrix mit genau den selben Parametern der gegebenen Matrix
	*/
	if(old == NULL){
		printf("can't duplicate a NULL pointer matrix\n");
		return NULL;
	}
	matrix * mx = malloc(sizeof(matrix));
	mx->m = old->m;
	mx->n = old->n;
	mx->elements = malloc((mx->n*mx->m)*sizeof(int));
	for(int i=0; i<mx->n*mx->m; i++){
		mx->elements[i] = old->elements[i];
	}
	return mx;
}

void add_matrix(matrix* a, matrix* b)
{
	/*TODO:
			-Argumente auf Gueltigkeit ueberpruefen
			-Elementweises Addieren von a und b (a+b)
	*/
	if(a == NULL || b == NULL){
		printf("one of the matrices is uninitialized\n");
		return;
	}
	if(a->m != b->m){
		printf("the matrices have different width\n");
		return;
	}
	if(a->n != b->n){
		printf("the matrices have different height\n");
		return;
	}
	if(a->elements == NULL || b->elements == NULL){
		printf("one of the matrices have uninitialized array of elements\n");
		return;
	}

	for(int i=0; i<a->m*a->n; i++){
		a->elements[i] += b->elements[i];
	}

}

void subtract_matrix(matrix* a, matrix* b)
{
	/*TODO:
			-Argumente auf Gueltigkeit ueberpruefen
			-Elementweises Subtrahieren von a und b (a-b)
	*/
	if(a == NULL || b == NULL){
		printf("one of the matrices is uninitialized\n");
		return;
	}
	if(a->m != b->m){
		printf("the matrices have different width\n");
		return;
	}
	if(a->n != b->n){
		printf("the matrices have different height\n");
		return;
	}
	if(a->elements == NULL || b->elements == NULL){
		printf("one of the matrices have uninitialized array of elements\n");
		return;
	}

	for(int i=0; i<a->m*a->n; i++){
		a->elements[i] -= b->elements[i];
	}
	
}

int get_elem_of_matrix(matrix* matrix, unsigned int i, unsigned int j)
{
	/*TODO
		-Argumente auf Gueltigkeit ueberpruefen
		-Rueckgabe des Elements an den Indizes i und j	
	*/
	if(matrix == NULL){
		printf("uninitialized matrix. returning -1\n");
		return -1;
	}
	if(i < 0 || i >= matrix->m || j < 0 || j >= matrix->n){
		printf("one of the given dimensions is invalid\n");
		return -1;
	}
	if(matrix->elements == NULL){
		printf("elements array in matrix is NULL\n");
		return -1;
	}
	
	return matrix->elements[i*matrix->n+j];
}

int check_matrix_elements(matrix* matrix)
{
	/*TODO:
			-Argumente auf Gueltigkeit ueberpruefen
			-Pruefen, ob die Elemente der Matrix <, > oder = 0 sind
			->sobald mindestens ein Element <0 ist, soll -1 zurückgegeben werden
			->wenn alle Elemente 0 sind, soll 0 zurückgegeben werden
			->ansonsten soll 1 zurückgegeben werden
	*/
	int all_positive = 0;

	if(matrix == NULL || matrix->elements == NULL){
		printf("matrix to check or its elements array is NULL\n");
		return -1;
	}
	for(int i=0; i<matrix->n*matrix->m;i++){
		if(matrix->elements[i] < 0) return -1;
		if(matrix->elements[i] > 0) all_positive = 1;
	}
	return all_positive;
}

matrix* get_row_from_matrix(matrix* mat, unsigned int m)
{
	/*TODO:
			-Argumente auf Gueltigkeit ueberpruefen
			-Rueckgabe der m-ten Zeile der Matrix (Die Zeile selbst ist auch wieder eine Matrix mit genau einer Zeile)
	*/
	if(mat == NULL || mat->elements == NULL){
		printf("mat or mat elements array is NULL pointer\n");
		return NULL;
	}
	if(m < 0 || m >= mat->m){
		printf("given m is out of range\n");
		return NULL;
	}
	
	matrix * mx = malloc(sizeof(matrix));
	mx->m = 1;
	mx->n = mat->n;
	mx->elements = malloc(mat->n*sizeof(int));
	for(int i=0; i<mat->n; i++){
		mx->elements[i] = mat->elements[i+m*mat->n];
	}
	return mx;
}
void print_matrix(matrix* matrix)
{
	if(matrix == NULL)
	{
		printf("ERROR: print_matrix() | given matrix is NULL\n");
		return;
	}


	//determines the largest element (for formatting the output with field widths)
	int max = get_elem_of_matrix(matrix, 0,0);	
	for(int i = 0; i< matrix->m; i++)
	{
		for(int j = 0; j < matrix->n; j++)
		{
			int current = get_elem_of_matrix(matrix, i,j);
			max = (current>max) ? current : max;
		}
	}

	int grade = 1;	//represents the number of digits of the largest element (for formatting the output with field widths)
	while(max > 9)
	{
		max/=10;
		grade++;
	}

	//prints upper line
	printf(" ");
	for(int i = 0; i< ((grade + 1)*matrix->n +1); i++)
	{
		printf("-");
	}
	printf("\n");

	//prints body
	for(int i = 0; i< matrix->m; i++)
	{
		printf("| ");
		for(int j = 0; j < matrix->n; j++)
		{
			printf("%*i ", grade, get_elem_of_matrix(matrix, i, j));
		}
		printf("|\n");
	}

	//prints lower line
	printf(" ");
	for(int i = 0; i< ((grade + 1)*matrix->n +1); i++)
	{
		printf("-");
	}
	printf("\n");
}


/*TODO:
	Hier koennen bei Bedarf eigene Funktionen implementiert werden
*/

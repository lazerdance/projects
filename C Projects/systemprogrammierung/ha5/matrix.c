#include "matrix.h"

//eigene Funktion
int lengthInt(int* array){
	return (sizeof(array)/sizeof(int));
}

matrix* create_matrix(unsigned int m, unsigned int n, int* numbers)
{
	/*TODO:
			-Argumente auf Gueltigkeit ueberpruefen
			-neue Matrix erstellen und zurückgeben (Pointer!!!)
			-(das Array soll dabei zwar kopiert, aber ansonsten UNVERAENDERT abgespeichert werden.
			  Dies ist für die automatischen Tests unerlässlich)
	*/
	if (numbers == NULL){
		exit(EXIT_FAILURE);
	}
	if (m*n != lengthInt(numbers)){
		exit(EXIT_FAILURE);
	}
	matrix* new_matrix = malloc(sizeof(matrix));
	int* copy_elements = malloc(sizeof(int) * lengthInt(numbers));
	if (new_matrix == NULL || copy_elements == NULL){
		exit(EXIT_FAILURE);
	}
	memcpy(copy_elements, numbers, (sizeof(copy_elements)));
	new_matrix->m = m;
	new_matrix->n = n;
	new_matrix->elements = copy_elements;
	return new_matrix;
}

matrix* create_matrix_from_row(unsigned int m, unsigned int row_nr, matrix* row)
{
	/*TODO
			-Argumente auf Gueltigkeit ueberpruefen
			-Erstellen einer neuen Matrix (Initialisierung mit 0)
			-Kopieren der gegebenen Zeile an die richtige Stelle
	*/
	if (row == NULL){
		exit(EXIT_FAILURE);
	}
	if (m <= row_nr){
		exit(EXIT_FAILURE);
	}
	matrix* new_matrix = malloc(sizeof(matrix));
	int* copy_elements = malloc(sizeof(int) * m * lengthInt(row->elements));
	if (new_matrix == NULL || copy_elements == NULL){
		exit(EXIT_FAILURE);
	}
	for(int i = 0; i < m * lengthInt(row->elements); i++){		//lengthInt(row->elements) entspricht neuem n
		if ( (i/lengthInt(row->elements))+1 == row_nr ) {		//Überprüfung ob richtige Reihe
			new_matrix->elements[i] = row->elements[(i % lengthInt(row->elements))];
		}
		new_matrix->elements[i] = 0;
	}  
	new_matrix->m = m;
	new_matrix->n = lengthInt(row->elements);
	return new_matrix;
}

void free_matrix(matrix* matrix)
{
	/*TODO:
			-Argumente auf Gueltigkeit ueberpruefen
			-Matrix und alle dynamisch reservierten elemente der Matrix freigeben
	*/
	if (matrix == NULL) {
		exit(EXIT_FAILURE);
	}
	free(matrix->elements);
	free(matrix);
}

matrix* duplicate_matrix(matrix* old)
{
	/*TODO:
			-Argumente auf Gueltigkeit ueberpruefen
			-Erstellen und Zurückgeben einer NEUEN Matrix mit genau den selben Parametern der gegebenen Matrix
	*/
	if ( old == NULL ){
		exit(EXIT_FAILURE);
	}
	if (old->elements == NULL){
		exit(EXIT_FAILURE);
	}
	matrix* new_matrix = malloc(sizeof(matrix));
	int* copy_elements = malloc(sizeof(int) * lengthInt(old->elements));
	if (new_matrix == NULL || copy_elements == NULL){
		exit(EXIT_FAILURE);
	}
	memcpy(copy_elements, old->elements, (sizeof(copy_elements)));
	new_matrix->m = old->m;
	new_matrix->n = old->n;
	new_matrix->elements = copy_elements;
	return new_matrix;
}

matrix* add_matrix(matrix* a, matrix* b)
{
	/*TODO:
			-Argumente auf Gueltigkeit ueberpruefen
			-Elementweises Addieren von a und b (a+b)
			-Rueckgabe des Ergebnisses
	*/
	if (a == NULL || b == NULL){
		exit(EXIT_FAILURE);
	}
	if ( a->elements == NULL || b->elements == NULL){
		exit(EXIT_FAILURE);
	}
	if ( a->m != b->m || a->n != b->n || lengthInt(a->elements) != lengthInt(b->elements) ){
		exit(EXIT_FAILURE);
	}
	if ( a->m * a->n != lengthInt(a->elements) ){
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < lengthInt(a->elements); i++){
		a->elements[i] = a->elements[i] + b->elements[i];
	}
	return a;
}

matrix* subtract_matrix(matrix* a, matrix* b)
{
	/*TODO:
			-Argumente auf Gueltigkeit ueberpruefen
			-Elementweises Subtrahieren von a und b (a-b)
			-Rueckgabe des Ergebnisses
	*/
	if (a == NULL || b == NULL){
		exit(EXIT_FAILURE);
	}
	if ( a->elements == NULL || b->elements == NULL){
		exit(EXIT_FAILURE);
	}
	if ( a->m != b->m || a->n != b->n || lengthInt(a->elements) != lengthInt(b->elements) ){
		exit(EXIT_FAILURE);
	}
	if ( a->m * a->n != lengthInt(a->elements) ){
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < lengthInt(a->elements); i++){
		a->elements[i] = a->elements[i] - b->elements[i];
	}
	return a;
}

int get_elem_of_matrix(matrix* matrix, unsigned int i, unsigned int j)
{
	/*TODO
		-Argumente auf Gueltigkeit ueberpruefen
		-Rueckgabe des Elements an den Indizes i und j	
	*/
	if (matrix == NULL) {
		exit(EXIT_FAILURE);
	}
	if (matrix->elements == NULL){
		exit(EXIT_FAILURE);
	}
	if ( matrix->m * matrix->n != lengthInt(matrix->elements) || matrix->m <= i || matrix->n <= j){
		exit(EXIT_FAILURE);
	}
	return matrix->elements[(i*(matrix->n))+j];
}

int check_matrix(matrix* matrix)
{
	/*TODO:
			-Argumente auf Gueltigkeit ueberpruefen
			-Pruefen, ob die Elemente der Matrix <, > oder = 0 sind
			->sobald ein Element <0 ist, soll -1 zurückgegeben werden
			->wenn alle Elemente 0 sind, soll 0 zurückgegeben werden
			->ansonsten soll 1 zurückgegeben werden
	*/
	if ( matrix == NULL ){
		exit(EXIT_FAILURE);
	}
	if (matrix->elements == NULL){
		exit(EXIT_FAILURE);
	}
	if ( matrix->m * matrix->n != lengthInt(matrix->elements) ){
		exit(EXIT_FAILURE);
	}
	int return_wert = 0;
	for(int i = 0; i < lengthInt(matrix->elements); i++){
		if ( matrix->elements[i] < 0 ){
			return -1;
		}
		if ( matrix->elements[i] > 0 ){
			return_wert = 1;
		}
	}
	return return_wert;
}

matrix* get_row_from_matrix(matrix* mat, unsigned int m)
{
	/*TODO:
			-Argumente auf Gueltigkeit ueberpruefen
			-Rueckgabe der m-ten Zeile der Matrix (Die Zeile selbst ist auch wieder eine Matrix mit genau einer Zeile)
	*/
	if ( mat == NULL ){
		exit(EXIT_FAILURE);
	}
	if (mat->elements == NULL){
		exit(EXIT_FAILURE);
	}
	if ( mat->m <= m ){
		exit(EXIT_FAILURE);
	}
	matrix* new_matrix = malloc(sizeof(matrix));
	int* copy_elements = malloc(sizeof(int) * (mat->n));
	if (new_matrix == NULL || copy_elements == NULL){
		exit(EXIT_FAILURE);
	}
	int start = mat->n * m;
	for(int i = 0; i < mat->n; i++){
		copy_elements[i] = mat->elements[i+start];
	}
	new_matrix->m = 1;
	new_matrix->n = mat->n;
	new_matrix->elements = copy_elements;
	return new_matrix;
}

void clear_row_of_matrix(matrix* matrix, unsigned int m)
{
	/*TODO
			-Argumente auf Gueltigkeit ueberpruefen
			-Die m-te Zeile der gegebenen Matrix nullen (alle Einträge auf '0' setzen)
	*/
	if ( matrix == NULL ){
		exit(EXIT_FAILURE);
	}
	if (matrix->elements == NULL){
		exit(EXIT_FAILURE);
	}
	if ( matrix->m <= m ){
		exit(EXIT_FAILURE);
	}
	int start = matrix->n * m;
	for(int i = 0; i < matrix->n; i++){
		matrix->elements[i+start] = 0;
	}
}

void print_matrix(matrix* matrix)
{
	if(matrix == NULL)
	{
		printf("ERROR: print_matrix() | given matrix is NULL\n");
		return;
	}


	//determines the largest element
	int max = get_elem_of_matrix(matrix, 0,0);
	for(int i = 0; i< matrix->m; i++)
	{
		for(int j = 0; j < matrix->n; j++)
		{
			int current = get_elem_of_matrix(matrix, i,j);
			max = (current>max) ? current : max;
		}
	}

	int grade = 1;	//represents the number of digits of the largest element
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


//===============================================================================

/*TODO:
	Hier koennen bei Bedarf eigene Funktionen implementiert werden
*/

//===============================================================================

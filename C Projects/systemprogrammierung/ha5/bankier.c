#include "bankier.h"

//===============================================================================
// Eigene Funktionen
/*
	Prozess "blockieren" (Belegungen wieder freigeben, Restanforderung auf Gesamtanforderung setzen)
*/
void blockieren(matrix* B, matrix* R, matrix* f, unsigned int ID){
	clear_row_of_matrix(R, ID);
	matrix* B_row = get_row_from_matrix(B, ID);
	add_matrix(f,B_row);
	clear_row_of_matrix(B, ID);
	free_matrix(B_row);
}
/*
	Anforderung erfüllen und ggf. den Prozess beenden (Belegungen wieder freigeben, Restanforderung auf 0 setzen)

	returns 	1 Prozess beendet
				0 Prozess nicht beendet
*/
int anforderungen_abarbeiten(matrix* B, matrix* R, matrix* f, matrix* allocation, unsigned int ID){
	int return_wert = 0;
	//Anforderungen erfüllen
	matrix* alloc_row = get_row_from_matrix(allocation, ID);
	subtract_matrix(f, alloc_row);
	subtract_matrix(R, allocation);
	add_matrix(B, allocation);
	//Prozess fertig? -> Belegung freigeben
	matrix* R_row = get_row_from_matrix(R, ID);
	if(check_matrix(R_row) == 0){
		matrix* B_row = get_row_from_matrix(B, ID);
		add_matrix(f, B_row);
		clear_row_of_matrix(B, ID);
		free_matrix(B_row);
		return_wert = 1;
	}
	else{
		return_wert = 0;
	}
	free_matrix(R_row);
	free_matrix(alloc_row);
	return return_wert;
}
//===============================================================================

int check_initial_state(matrix* f, matrix* G)
{
	/*TODO
			Testen, ob kein Prozess mehr Resourcen benötigt, als insgesamt verfügbar sind
	*/
	if (f == NULL || G == NULL){
		exit(EXIT_FAILURE);
	}
	if ( f->n != G->n){
		exit(EXIT_FAILURE);
	}
	for(int i = 0; i < (G->m * G->n); i = i + f->n){	//Schleife startet immer am Anfang einer Reihe
		for(int j = 0; j < f->n; j++){					//Schleife geht die Reihe durch
			if(G->elements[i+j] > f->elements[j]){
				return 0;
			}
		}
	}
	return 1;
}

int check_allocation(matrix* B, matrix* R, matrix* f, matrix* allocation, unsigned int ID)
{
	/*TODO:
			-Testen, ob überhaupt genügend Ressourcen vorhanden sind, um Anforderung zu erfüllen
			-"virtuelle Umgebung" für den Bankieralgorithmus schaffen
			-Bankieralgorithmus aufrufen
			-Bei Erfolg: Anforderung erfüllen und ggf. den Prozess 
			 beenden (Belegungen wieder freigeben, Restanforderung auf 0 setzen)
			->Ansonsten den Prozess "blockieren" (Belegungen wieder freigeben, Restanforderung auf 0 setzen)
			-Erinnerung: dynamisch reservierter Speicher muss wieder freigegeben werden!
	*/
	// Eingabe überprüfen +++++++++++++++++++++++++++++ Evlt noch überprüfen, ob allocation > Restanforderung ++++++++++++++++++++++++++
	if ( B == NULL || R == NULL || f == NULL || allocation == NULL){
		exit(EXIT_FAILURE);
	}
	// Testen, ob Anforderung erfüllbar ist
	//Startet immer am Anfang der Reihe 
	for( int j = 0; j < allocation->n; j++){ 		//geht die Reihe an der ID durch
		if ( allocation->elements[(ID * allocation->n)+j] != 0 && allocation->elements[(ID * allocation->n)+j] > f->elements[j]) {
			blockieren(B, R, f, ID);
			return 0;
		}
	}
	//virtuelle Umgebung für Bankieralgorithmus schaffen
	matrix* cpyB = duplicate_matrix(B);
	matrix* cpyR = duplicate_matrix(R);
	matrix* cpyf = duplicate_matrix(f);
	//Bankieralgorithmus aufrufen ++++++++++++++++++++++++ Mit oder ohne der neuen Allocation? ++++++++++++++++++++++++++++++++++++++++
	int ergebnis = 0;
	ergebnis = check_state(cpyB, cpyR, cpyf);
	if(ergebnis){	// Erfolg, Anforderungen erfüllen, evtl Prozess beenden
		anforderungen_abarbeiten(B, R, f, allocation, ID);
	}
	else{	// Misserfolg, Prozess blockieren
		blockieren(B, R, f, ID);
	}
	//alles freigeben
	free_matrix(cpyB);
	free_matrix(cpyR);
	free_matrix(cpyf);
	return -1;
}

int check_state(matrix* B, matrix* R, matrix* f)
{
	/*TODO:
			-Bankieralgorithmus implementieren
	*/
	// Liste erstellen, welche Prozesse laufen konnten.
	const int num_processes = (int) B->m;
	int runList[num_processes];					 //Bsp if run[2] = 1 -> 3. Prozess ist gelaufen.( = {0} will nicht funktionieren?)
	for (int p = 0; p < num_processes; p++){	 // = {0} ersatz
		runList[p] = 0;
	}
	//Bankieralgorithmus alle Prozesse (Reihen) durchgehen
	int can_run = 1; 					
	for( int i = 0; i < B->m; i++){ 			//Reihenindex durchgehen
		if (runList[i] == 0){					//Testen ob Prozess mit Index i schon gelaufen ist
			for (int j = 0; j < B->n; j++){		//Spaltenindex durchgehen
				//Überprüfen ob Prozess Index i laufen kann
				if( R->elements[i+j] > f->elements[j] ){
					can_run = 0;
				}
			}
			if (can_run == 1){
				//Anforderungen erfüllen
				matrix* R_row = get_row_from_matrix(R, i);
				if (anforderungen_abarbeiten(B, R, f, R_row, i) == 0) {
					printf("Fehler Bankieralgorithmus: Prozess der vollständig laufen kann wurde trotzdem nicht beendet!");
				}
				free_matrix(R_row);
				runList[i] = 1;					//Prozesse als gelaufen merken und
				i = 0;							//von vorne anfangen
			}
			can_run = 1;
		}
		//Überprüfen, ob alle Prozesse gelaufen sind -> sicherer Zustand
		int all_ran = 1;
		for( int p = 0; p < num_processes; p++ ){
			if (runList[p] != 1){
				all_ran = 0;
				//break;
			}
		}
		if (all_ran == 1){
				return 1;
			}
	}
	//Unsicherer Zustand
	return 0;
}

int abs(int n)
{
	return (n>0) ? n : -n;
}

void print_BRf(matrix* B, matrix* R, matrix* f)
{
	unsigned int m = B->m;
	unsigned int n = B->n;

	if(B==NULL || R==NULL || f==NULL)
	{
		printf("ERROR: print_BRf() | given matrices = NULL.\n");
		return;
	}

	if(f->m != 1 || f->n != n || R->m !=m || R->n != n)
	{
		printf("ERROR: print_BRf() | given matrices dont have matching dimensions.\n");
		return;
	}

	int max_B = get_elem_of_matrix(B, 0,0);		//largest elements
	int max_R = get_elem_of_matrix(R, 0,0);
	int max_f = get_elem_of_matrix(f, 0,0);

	int grade_B = 1;	//represents the number of digits of the largest element
	int grade_R = 1;
	int grade_f = 1;

	int width_B;
	int width_R;
	int width_f;


	for(unsigned int i = 0; i< B->m; i++)
	{
		for(unsigned int j = 0; j < B->n; j++)
		{
			int current = get_elem_of_matrix(B, i,j);
			max_B = (abs(current)>max_B) ? current : max_B;
		}
	}

	for(unsigned int i = 0; i< R->m; i++)
	{
		for(unsigned int j = 0; j < R->n; j++)
		{
			int current = get_elem_of_matrix(R, i,j);
			max_R = (abs(current)>max_R) ? current : max_R;
		}
	}

	for(unsigned int j = 0; j < f->n; j++)
	{
		int current = get_elem_of_matrix(f, 0,j);
		max_f = (abs(current)>max_f) ? current : max_f;
	}

	if(max_B < 0) grade_B++;
	while(max_B > 9 || max_B < -9)
	{
		max_B/=10;
		grade_B++;
	}
	width_B = 3 + n*(grade_B+1);

	if(max_R < 0) grade_R++;
	while(max_R > 9 || max_R < -9)
	{
		max_R/=10;
		grade_R++;
	}
	width_R = 3 + n*(grade_R+1);

	if(max_f < 0) grade_f++;
	while(max_f > 9 || max_f < -9)
	{
		max_f/=10;
		grade_f++;
	}
	width_f = 3 + n*(grade_f+1);

	//print upper line (B: R: f:)
	for(int i = 0; i < width_B + width_R + width_f + 2*3; i++)
	{
		if(i==(int)((width_B-1)/2)) printf("B");
		else if(i==(int)(width_B + 3 + (width_R-1)/2)) printf("R");
		else if(i==(int)(width_B + 3 + width_R + (width_f-1)/2)) printf("f");
		else printf(" ");
	}
	printf("\n");

	//================================ print B ================================= 
	//prints upper line
	printf(" ");
	for(int i = 0; i< width_B-2; i++) 	printf("-");

	printf("\n");

	//prints body
	for(unsigned int i = 0; i< m; i++)
	{
		printf("| ");
		for(unsigned int j = 0; j < n; j++)
		{
			int elem = get_elem_of_matrix(B, i, j);
			if(elem < 0) printf("\033[1;31m%*i\033[0m ", grade_B, elem);
			else 		 printf("%*i ", grade_B, elem);
			
		}
		printf("|\n");
	}

	//prints lower line
	printf(" ");
	for(int i = 0; i< width_B-2; i++)	printf("-");
	
	printf("\r\033[%iC\033[%iA", (width_B+3), (m+1));	//move curser forwards and upwards

	//================================ print R ================================= 
	//prints upper line
	printf(" ");
	for(int i = 0; i< width_R-2; i++) 	printf("-");

	printf("\033[%iD\033[%iB", (width_R-1), 1);		//move curser backwards and downwards

	//prints body
	for(int i = 0; i< m; i++)
	{
		printf("| ");
		for(int j = 0; j < n; j++)
		{
			int elem = get_elem_of_matrix(R, i, j);
			if(elem < 0) printf("\033[1;31m%*i\033[0m ", grade_R, elem);
			else 		 printf("%*i ", grade_R, elem);
		}
		printf("|\033[%iD\033[%iB", width_R, 1);	//move curser backwards and downwards
	}

	//prints lower line
	printf(" ");
	for(int i = 0; i< width_R-2; i++)	printf("-");
	
	printf("\033[%iC\033[%iA", 4, (int)(m/2 +2));		//move curser forwards and upwards


	//================================ print f ================================= 
	//prints upper line
	printf(" ");
	for(int i = 0; i< width_f-2; i++) 	printf("-");

	printf("\033[%iD\033[%iB", (width_f-1), 1);		//move curser backwards and downwards

	//prints body
	printf("| ");
	for(int j = 0; j < n; j++)
	{
		int elem = get_elem_of_matrix(f, 0, j);
		if(elem < 0) printf("\033[1;31%*i\033[0m ", grade_f, elem);
		else 		 printf("%*i ", grade_f, elem);
	}
	printf("|\033[%iD\033[%iB", width_f, 1);	//move curser backwards and downwards

	//prints lower line
	printf(" ");
	for(int i = 0; i< width_f-2; i++)	printf("-");
	
	printf("\r\033[%iB\n\n", (int)(m/2 +1));	//move curser backwards and downwards

}
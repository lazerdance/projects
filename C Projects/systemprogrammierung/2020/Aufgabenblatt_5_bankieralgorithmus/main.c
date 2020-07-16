#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "libs/parser.h"
#include "matrix.h"
#include "bankier.h"

/*
	TLDR: 	Die main() liest die als Parameter angegebenen Daten ein, verarbeitet sie und speichert sie ab.
			Anschließend betritt das Programm eine while-Schleife, in der nach und nach jede Anfrage auf
			Erfüllbarkeit überprüft wird.

			DIE MAIN() IST BEREITS FUNKTIONSFÄHIG UND MUSS NICHT BEARBEITET WERDEN! (good joke lol)

*/

state* data;	//struct that holds all the data retrieved from data.txt

int num_of_allocations;	//number of pending allocations
int m;			//number of processes
int n;			//number of resources

matrix* G;			//max (Gesamtanforderung)
matrix* f;			//available (Vektor freier Ressourcen)
matrix* R; 			//Need (Restanforderung)
matrix* B;			//Allocation (Belegung)
queue* allocation_list;	//queue containing all the pending allocations (as matrices) (pointer)
queue al_Q;				//(the actual queue)

int* blocked_processes;	//array to hold information about the status of all processes (shared variable, is used in bankier.c as well)
						// 0 = process running
						// 1 = process blocked


int retrieve_data_from_file(char* file_path)
{
	if((data = read_data(file_path))== NULL)	//reads all the data from the given file and returns it within a struct
	{
		printf("encountered error during parsing, exiting. Please check [%s].\n", file_path);
		return 0;
	}

	//extracts the information from the struct
	m = data->m;
	n = data->n;
	num_of_allocations = data->allocations->size;
	f = create_matrix(1, n, (int*)(data->f));

	int* tmp_zeros = (int*) malloc(m*n*sizeof(int));	//alloc int array for creation of B-matrix
	memset(tmp_zeros, 0, m*n*sizeof(int)); 				//initialize the int-array with zeros
	B = create_matrix(m, n, tmp_zeros);					//init B and G with zeros (G is added up later)
	G = create_matrix(m, n, tmp_zeros);					
	free(tmp_zeros);


	//initializes and fills the list containing the allocations (copies information)
	allocation_list = init_Q(&al_Q);
	node* alloc_list_element = data->allocations->head;
	for(int i =0; i< num_of_allocations; i++)
	{
		alloc_obj* old = (alloc_obj*)(alloc_list_element->data);	//old allocation object
		matrix* row = create_matrix(1, n, old->row->elements);		//copies values into new matrix (old matrix will be freed)
		alloc_obj* new = create_alloc_obj(row, old->ID);
		append_node(create_node(new, sizeof(alloc_obj)), allocation_list);	//creates new list node and appends it to alloc_list

		alloc_list_element = alloc_list_element->prev;				//sets pointer to next elem

		matrix* tmp = create_matrix_from_row(m, new->ID, row);
		add_matrix(G, tmp);		//adds the parsed allocation to G
		free_matrix(tmp);
	}

	R = duplicate_matrix(G);

	/*printf("data read: \n");
	print_state(data);*/

	free_state(data);

	printf("\n\033[1mData parsed:\033[0m\n");
	printf("m: %u\nn: %u\n\n   G:\n",m,n);
	print_matrix(G);
	printf("\n   f:\n");
	print_matrix(f);
	
	return 1;

}

int main(int argc, char ** argv)
{
	int mode = 0;
	//======================================

	if(argc != 2 && argc != 3)
	{
		printf("Invalid Number of parameters. Try: ./banker [path/to/data.txt] [mode]\n");
		printf("[mode]=0: once started, the simulation runs until the allocation queue is empty\n");
		printf("[mode]=1: the simulation waits after each allocation until the user presses 'enter'\n");
		return 0;
	}

	if(argc == 3) mode = atoi(argv[2]);		//select mode
	if(mode != 0 && mode != 1) mode = 0;

	printf("====================================================\n");

	//reading data from file
	if(!retrieve_data_from_file(argv[1]))
	{
		return 0;
	}

	//checks whether with the given amount of resources any process could be executed at all
	if(!check_initial_state(f, G))
	{
		printf("Allocation sequence failed: At least one process requires more resources than available\n");

		//frees the allocation list
		node* alloc = allocation_list->head;
		while(alloc!=NULL)
		{
			free_alloc_obj(alloc->data);
			alloc = alloc->prev;
		}
		free_Q(allocation_list);

		free_matrix(G);
		free_matrix(R);
		free_matrix(f);
		free_matrix(B);

		return 0;
	}

	blocked_processes = (int*) malloc(m*sizeof(int));		//allocate and init blocked_processes array
	memset(blocked_processes, 0, sizeof(int)*m);

	printf("\n=====================================================\n");
	printf("\nCurrent state:\n");
	print_BRf(B, R, f);


	printf("\n\033[1mPress 'enter' to continue\033[0m\n");	//wait for user to confirm
	while(1)
	{
		int char_read = getchar();
		if(char_read == '\n')
		{
			printf("\033[1A\033[2K\033[1A\033[2K");			//erase the "press enter to continue" line
			break;
		}
	}

	//================================================	work off all the pending allocations
	node* current_node  = allocation_list->head;
	node* next_node;

	while(current_node  != NULL)
	{
		alloc_obj* current_alloc = (alloc_obj*)current_node->data;
		int ID = current_alloc->ID;
		printf("Trying to execute allocation:\n\nID: %i\n", ID);
		print_matrix(current_alloc->row);
		printf("\n");

		next_node  = current_node->prev;

		
		if(blocked_processes[ID])
		{
			printf("Skipped allocation of process %i (already blocked)\n", ID);
			dequeue_allocation(allocation_list, current_node);
		}
		else
		{
			int valid = check_allocation(B, R, f, current_alloc->row, ID);

			if(valid)		//allocation was secure
			{
				printf("Allocation \033[42msuccessful\033[0m!\n\n");
				printf("Current state:\n");
				print_BRf(B, R, f);
				dequeue_allocation(allocation_list, current_node);
				num_of_allocations--;
			} 
			else 			//allocation was risky --> block process
			{
				printf("Allocation \033[41mfailed\033[0m! Blocking process %i.\n\n", ID);
				blocked_processes[ID] = 1;		//marking process as "blocked"
				
				printf("Current state:\n");
				print_BRf(B, R, f);
				dequeue_allocation(allocation_list, current_node);
			}
		}
		
		printf("\n=====================================================\n");

		if(mode==0)
		{
			sleep(1);
		}
		else
		{
			printf("\n\033[1mPress 'enter' to continue\033[0m\n");	//wait for user to confirm
			while(1)
			{
				int char_read = getchar();
				if(char_read == '\n')
				{
					printf("\033[1A\033[2K\033[1A\033[2K"); 		//erase the "press enter to continue" line
					break;
				}
			}
		}
		current_node = next_node ;
	}

	//==============================================================================

	printf("All allocations are either executed or rejected. Exiting...\n\n");

	//frees the allocation list (it should be empty at this point, but who knows...)
	node* alloc = allocation_list->head;
	while(alloc!=NULL)
	{
		free_alloc_obj((alloc_obj*)alloc->data);
		alloc = alloc->prev;
	}
	free(blocked_processes);
	free_Q(allocation_list);

	free_matrix(G);
	free_matrix(R);
	free_matrix(f);
	free_matrix(B);
	return 0;
}

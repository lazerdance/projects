#include "parser.h"

int parser_m = 0;
int parser_n = 0;

int tmp_array[100];
int num_of_A = 0;

state* create_state(int m, int n)
{
	state* s = (state*)malloc(sizeof(state));
	s->m = m;
	s->n = n;
	s->f = (int*)malloc(sizeof(int)*n);
	init_Q(&s->a_array);
	s->allocations = &s->a_array;

	return s;
}

void free_state(state* s)
{
	node* elem = s->allocations->head;
	while(elem!=NULL)
	{
		//free_matrix((matrix*)((alloc_obj*)elem->data)->row);
		free_alloc_obj((alloc_obj*)elem->data);
		elem = elem->prev;
	}
	free_Q(s->allocations);

	free(s->f);
	free(s);
}


alloc_obj* create_alloc_obj(matrix* row, int ID)
{
	alloc_obj* a = (alloc_obj*) malloc(sizeof(alloc_obj));
	a->row = row;
	a->ID = ID;

	return a;
}


void free_alloc_obj(alloc_obj* a)
{
	if(a->row != NULL) free_matrix(a->row);
	free(a);
}


int fill_array(char* to_read, int* to_fill, int num)
{

	if(num<1)
	{
		printf("WARNING: detected array size than less one!\n");
		return 0;
	}

	char* ptr;
	ptr = strtok(to_read, ",;}");
	to_fill[0] = atoi((const char*)ptr);

	for(int i=1; i<num; i++)
	{
		ptr = strtok(NULL, ",;}");
		if(ptr == NULL)
		{
			printf("WARNING: an array parsed probably did not hold as many elements as expected!\n");
			return -1;
		} 
		to_fill[i] = atoi((const char*)ptr);
	}

	return 1;
}

state* read_data(char* name)
{
	FILE* file = fopen(name, "r");
	char line[300];
	int ID;

	if(file==NULL)
	{
		printf("ERROR: file not found\n");
		return NULL;
	}

	state* current_state = NULL;

	while(fgets(line, sizeof(line), file) != NULL)
	{
		if(parser_m!=0 && parser_n!=0 && (current_state == NULL))
		{
			current_state = create_state(parser_m,parser_n);
		} 

		switch(line[0])
		{
			case 'm':	parser_m=atoi((const char*)(line+2));
						if(parser_m<1 || parser_m>9)
						{
							printf("ERROR: variable 'm' not in range [1 - 9]\n");
							fclose(file);
							return NULL;
						}
						break;

			case 'n':	parser_n=atoi((const char*)(line+2));
						if(parser_n<1 || parser_n>9)
						{
							printf("ERROR: variable 'n' not in range [1 - 9]\n");
							fclose(file);
							return NULL;
						}
						break;

			case 'f':	if(fill_array(line+3, current_state->f, parser_n)!=1)
						{
							printf("ERROR: detected a problem while parsing the f-array\n");
							return NULL;
						}
						break;
						
			case 'A':	ID=atoi((const char*)(line+2));
						if(fill_array(line+5, tmp_array, parser_n)!=1 || ID >= 9)
						{
							printf("ERROR: detected a problem while parsing an A-array\n");
							return NULL;
						}

						alloc_obj* alloc = create_alloc_obj(NULL, ID);
						alloc->row = create_matrix(1, parser_n, tmp_array);

						node* new_node = create_node(alloc, parser_m*parser_n*sizeof(int));		
						
						append_node(new_node, current_state->allocations);
						break;
			default: break;
		}
	}
	
	fclose(file);
	return current_state;
}

void dequeue_allocation(queue* q, node* allocation)
{
	remove_node(allocation, q);
	free_alloc_obj((alloc_obj*)allocation->data);
	free_node(allocation);
}

void move_allocation_to_back(queue* q, node* allocation)
{
	remove_node(allocation, q);
	append_node(allocation, q);
}

void print_array(int* a, int len)
{
	if(len <1) return;

	printf("[%i", a[0]);
	for(int i=1; i< len; i++)
	{
		printf(" --> %i", a[i]);
	}
	printf("]\n");
}

void print_alloc(alloc_obj* a, int alloc_num)
{
	if(a == NULL || a->row == NULL)
	{
		printf("ERROR: print_alloc() | given allocation or its payload is NULL\n");
		return;
	}

	matrix* matrix = a->row;


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
	printf("A%02i:  ", alloc_num);
	for(int i = 0; i< ((grade + 1)*matrix->n +1); i++)
	{
		printf("-");
	}
	printf("\n");

	printf("p%i:  | ", a->ID);
	for(int j = 0; j < matrix->n; j++)
	{
		printf("%*i ", grade, get_elem_of_matrix(matrix, 0, j));
	}
	printf("|\n");

	//prints lower line
	printf("      ");
	for(int i = 0; i< ((grade + 1)*matrix->n +1); i++)
	{
		printf("-");
	}
	printf("\n");
}

void print_state(state* s)
{
	printf("m: %i   |   n: %i\n", s->m, s->n);
	printf("f:  ");
	print_array(s->f, s->n);
	printf("\n");

	if(s->allocations == NULL)
	{
		printf("ERROR: print_state()\n");
		return;
	}

	node* tmp = s->allocations->head;
	int num = 0;
	while(tmp!=NULL)
	{
		//print_matrix((matrix*)((alloc_obj*)tmp->data)->row);
		print_alloc((alloc_obj*)tmp->data, num);
		printf("\n");
		tmp = tmp->prev;
		num++;
	}
	printf("\n");
}